#include "posit.h"
#include <math.h>

#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define LSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define RSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define POW2(n) \
    (LSHIFT(1, (n)))

#define HIDDEN_BIT(a) \
    ((a >> 1) | MSB)

#define MIN(a, b) \
    ((a) < (b) ? (a) : (b))

#define MAX(a, b) \
    ((a) > (b) ? (a) : (b))

const unsigned int MSB = 1u << 31u;
const unsigned int MASK = 0xFFFFFFFF;
const int ES = 2;
const int POSIT_SIZE = 32;
const int MAX_EXP = 30 * 16;

typedef struct unpacked_t {
    short neg;
    int exp;
    unsigned int frac;
} unpacked;

short _is_nar(posit p) {
    return p.bits == MSB;
}

short _is_neg(posit p) {
    return (int) p.bits < 0 && !_is_nar(p);
}

int _get_rs(posit p) {
    int lz = CLZ(LSHIFT(p.bits, 1));
    int lo = CLZ(LSHIFT(~p.bits, 1));
    int rs = MAX(lz, lo) + 1;

    return MIN(rs, POSIT_SIZE - 1);
}

unpacked unpackedPositFromDouble(double f) {
    unpacked up;
    int bias = 1023;

    union {
        double f;
        unsigned long long u;
    } un;

    un.f = f;
    up.neg = (short) RSHIFT(un.u, 63u);
    up.exp = (int) ((RSHIFT(un.u, 52u)) & 0x7FFu) - bias;
    up.frac = (unsigned int) (RSHIFT(LSHIFT(un.u, 12u), 32u));

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

posit positFromUnpacked(unpacked up) {
    if (up.exp < -MAX_EXP)
        up.exp = -MAX_EXP;
    else if (up.exp > MAX_EXP)
        up.exp = MAX_EXP;

    int reg = up.exp / POW2(ES) - (up.exp % POW2(ES) < 0);

    unsigned int ss = 1;
    unsigned int rs = MAX(-reg + 1, reg + 2);
    unsigned int exp = up.exp - POW2(ES) * reg;

    unsigned int regbits;
    if (reg < 0)
        regbits = RSHIFT(MSB, -reg);
    else
        regbits = LSHIFT(MASK, POSIT_SIZE - reg - 1);
    unsigned int expbits = LSHIFT(exp, POSIT_SIZE - ES);


    unsigned int tmp = RSHIFT(regbits, ss)
            | RSHIFT(expbits, ss + rs)
            | RSHIFT(up.frac, ss + rs + ES);

    if (up.neg)
        tmp = ~(tmp - 1);

    posit res = {tmp};
    return res;
}

posit positFromDouble(double f) {
    return positFromUnpacked(unpackedPositFromDouble(f));
}

unpacked unpackedFromPosit(posit p) {
    unpacked res;
    res.neg = _is_neg(p);

    if (res.neg)
        p.bits = (~p.bits) + 1;

    int rs = _get_rs(p);

    int lz = CLZ(LSHIFT(p.bits, 1));
    int lo = CLZ(LSHIFT(~p.bits, 1) | 1);

    int reg = (lz == 0 ? lo - 1 : -lz);
    int exp = RSHIFT(LSHIFT(p.bits, 1 + rs), POSIT_SIZE - ES);

    res.exp = POW2(ES) * reg + exp;
    res.frac = LSHIFT(p.bits, 1 + rs + ES);

    return res;
}

double toDouble(posit a) {
    unpacked up = unpackedFromPosit(a);

    int fexp = up.exp + 1023;

    unsigned long long fexpbits;
    unsigned long long ffracbits;

    if (fexp > 2046) {
        fexpbits = LSHIFT((unsigned long long) 2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = LSHIFT((unsigned long long)(MSB | RSHIFT(up.frac, 1)), 64 - POSIT_SIZE);
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((unsigned long long)(fexp & 0x7FF), 53);
        ffracbits = LSHIFT((unsigned long long) up.frac, 64 - POSIT_SIZE);
    }

    union {
        double f;
        unsigned long long u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((unsigned long long) up.neg, 63) | RSHIFT(un.u, 1);

    if (LSHIFT(un.u, 1) == 0)
        un.u++;

    return un.f;
}

unpacked addUnpacked(unpacked a, unpacked b, short neg) {
    unpacked r;

    unsigned long long afrac = HIDDEN_BIT(a.frac);
    unsigned long long bfrac = HIDDEN_BIT(b.frac);
    unsigned long long frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (RSHIFT(frac, POSIT_SIZE) != 0) {
        r.exp++;
        frac = RSHIFT(frac, 1);
    }

    r.neg = neg;
    r.frac = LSHIFT(frac, 1);

    return r;
}

unpacked subUnpacked(unpacked a, unpacked b, short neg) {
    unpacked r;

    unsigned long long afrac = HIDDEN_BIT(a.frac);
    unsigned long long bfrac = HIDDEN_BIT(b.frac);
    unsigned long long frac;

    if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        neg = !neg;
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
        frac = bfrac - afrac;
    }

    r.neg = neg;
    r.exp -= CLZ(frac);
    r.frac = LSHIFT(frac, CLZ(frac) + 1);

    return r;
}

short oppositeUnpacked(unpacked a, unpacked b, short neg) {
    if (a.exp == b.exp && a.frac == b.frac)
        return (a.neg + b.neg + neg) % 2;
    return 0;
}

posit add(posit ap, posit bp) {
    unpacked a = unpackedFromPosit(ap);
    unpacked b = unpackedFromPosit(bp);

    if (oppositeUnpacked(a, b, 0))
        return positFromDouble(0);
    else if (a.neg == b.neg)
        return positFromUnpacked(addUnpacked(a, b, a.neg));
    else
        return positFromUnpacked(subUnpacked(a, b, a.neg));
}

posit sub(posit ap, posit bp) {
    unpacked a = unpackedFromPosit(ap);
    unpacked b = unpackedFromPosit(bp);

    if (oppositeUnpacked(a, b, 1))
        return positFromDouble(0);
    else if (a.neg == b.neg)
        return positFromUnpacked(subUnpacked(a, b, a.neg));
    else
        return positFromUnpacked(addUnpacked(a, b, a.neg));
}

posit multiply(posit ap, posit bp) {
    unpacked a = unpackedFromPosit(ap);
    unpacked b = unpackedFromPosit(bp);
    unpacked r;

    unsigned long long afrac = HIDDEN_BIT(a.frac);
    unsigned long long bfrac = HIDDEN_BIT(b.frac);
    unsigned int frac = RSHIFT(afrac * bfrac, POSIT_SIZE);
    int exp = a.exp + b.exp + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return positFromUnpacked(r);
}

posit divide(posit ap, posit bp) {
    unpacked a = unpackedFromPosit(ap);
    unpacked b = unpackedFromPosit(bp);
    unpacked r;

    unsigned long long afrac = HIDDEN_BIT(a.frac);
    unsigned long long bfrac = HIDDEN_BIT(b.frac);
    int exp = a.exp - b.exp;

    if (afrac < bfrac) {
        exp--;
        bfrac = RSHIFT(bfrac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(afrac, POSIT_SIZE) / bfrac;

    return positFromUnpacked(r);
}

posit power(posit a, posit b) {
    return positFromDouble(pow(toDouble(a), toDouble(b)));
}

posit negate(posit a) {
    if (_is_neg(a)) {
        posit res = {(~a.bits) + 1};
        return res;
    }
    posit res = {~(a.bits - 1)};
    return res;
}

posit p_abs(posit ap) {
    unpacked a = unpackedFromPosit(ap);
    return a.neg ? negate(ap) : ap;
}

short is_less(posit rhs, posit lhs) {
    return toDouble(rhs) < toDouble(lhs);
}
