//#include <brain>
#include "posit.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define LSHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift)) //adding shifts to make it easier
#define RSHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))
#define HIDDEN_BIT(a) ((a >> 1) | MSB)
#define POW2(n) (LSHIFT(1, (n)))
#define uint64 unsigned long long
#define uint32 unsigned int
#define bool short

const uint32 MSB = 1u << 31u;
const uint32 MASK = 0xFFFFFFFF;
const int ES = 2;
const int POSIT_SIZE = 32;
const int MAX_EXP = 30 * 16;

typedef struct posit_information_t {
    bool neg;
    int exp;
    uint32 frac;
} posit_information;

bool is_nar(posit p) {
    return p.bits == MSB;
}

bool is_neg(posit p) {
    return (int) p.bits < 0 && !is_nar(p);
}

int get_rs(posit p) {
    int lz = CLZ(LSHIFT(p.bits, 1));
    int lo = CLZ(LSHIFT(~p.bits, 1));
    int rs = MAX(lz, lo) + 1;
    return MIN(rs, POSIT_SIZE - 1);
}

posit_information fromDoubleToPositInformation(double f) {
    posit_information up;
    int bias = 1023;

    union {
        double f;
        uint64 u;
    } un;

    un.f = f;
    up.neg = (bool) RSHIFT(un.u, 63u);
    up.exp = (int) ((RSHIFT(un.u, 52u)) & 0x7FFu) - bias;
    up.frac = (uint32) (RSHIFT(LSHIFT(un.u, 12u), 32u));

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

posit toPosit(posit_information up) {
    if (up.exp < -MAX_EXP)
        up.exp = -MAX_EXP;
    else if (up.exp > MAX_EXP)
        up.exp = MAX_EXP;

    int reg = up.exp / POW2(ES) - (up.exp % POW2(ES) < 0);

    uint32 ss = 1;
    uint32 rs = MAX(-reg + 1, reg + 2);
    uint32 exp = up.exp - POW2(ES) * reg;

    uint32 regbits;
    if (reg < 0)
        regbits = RSHIFT(MSB, -reg);
    else
        regbits = LSHIFT(MASK, POSIT_SIZE - reg - 1);
    uint32 expbits = LSHIFT(exp, POSIT_SIZE - ES);


    uint32 tmp = RSHIFT(regbits, ss)
                 | RSHIFT(expbits, ss + rs)
                 | RSHIFT(up.frac, ss + rs + ES);

    if (up.neg)
        tmp = ~(tmp - 1);

    posit res = {tmp};
    return res;
}

posit fromDouble(double f) {
    return toPosit(fromDoubleToPositInformation(f));
}

posit_information fromPosit(posit p) {
    posit_information res;
    res.neg = is_neg(p);

    if (res.neg)
        p.bits = (~p.bits) + 1;

    int rs = get_rs(p);

    int lz = CLZ(LSHIFT(p.bits, 1));
    int lo = CLZ(LSHIFT(~p.bits, 1) | 1);

    int reg = (lz == 0 ? lo - 1 : -lz);
    int exp = RSHIFT(LSHIFT(p.bits, 1 + rs), POSIT_SIZE - ES);

    res.exp = POW2(ES) * reg + exp;
    res.frac = LSHIFT(p.bits, 1 + rs + ES);

    return res;
}

double toDouble(posit a) {
    posit_information up = fromPosit(a);

    int fexp = up.exp + 1023;

    uint64 fexpbits;
    uint64 ffracbits;

    if (fexp > 2046) {
        fexpbits = (uint64_t) 2046 << 53;
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = LSHIFT((uint64)(MSB | RSHIFT(up.frac, 1)), 64 - POSIT_SIZE);
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((uint64)(fexp & 0x7FF), 53);
        ffracbits = LSHIFT((uint64) up.frac, 64 - POSIT_SIZE);
    }

    union {
        double f;
        uint64 u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((uint64) up.neg, 63) | RSHIFT(un.u, 1);

    if (LSHIFT(un.u, 1) == 0)
        un.u++;

    return un.f;
}

posit_information _add(posit_information a, posit_information b, bool neg) {
    posit_information r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    uint64 frac;

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

posit_information _sub(posit_information a, posit_information b, bool neg) {
    posit_information r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    uint64 frac;

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

bool _opposite(posit_information a, posit_information b, bool neg) {
    if (a.exp == b.exp && a.frac == b.frac)
        return (a.neg + b.neg + neg) % 2;
    return 0;
}

posit add(posit ap, posit bp) {
    posit_information a = fromPosit(ap);
    posit_information b = fromPosit(bp);

    if (_opposite(a, b, 0))
        return fromDouble(0);
    else if (a.neg == b.neg)
        return toPosit(_add(a, b, a.neg));
    else
        return toPosit(_sub(a, b, a.neg));
}

posit sub(posit ap, posit bp) {
    posit_information a = fromPosit(ap);
    posit_information b = fromPosit(bp);

    if (_opposite(a, b, 1))
        return fromDouble(0);
    else if (a.neg == b.neg)
        return toPosit(_sub(a, b, a.neg));
    else
        return toPosit(_add(a, b, a.neg));
}

posit multiply(posit ap, posit bp) {
    posit_information a = fromPosit(ap);
    posit_information b = fromPosit(bp);
    posit_information r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    uint32 frac = RSHIFT(afrac * bfrac, POSIT_SIZE);
    int exp = a.exp + b.exp + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return toPosit(r);
}

posit divide(posit ap, posit bp) {
    posit_information a = fromPosit(ap);
    posit_information b = fromPosit(bp);
    posit_information r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    int exp = a.exp - b.exp;

    if (afrac < bfrac) {
        exp--;
        bfrac = RSHIFT(bfrac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(afrac, POSIT_SIZE) / bfrac;

    return toPosit(r);
}

posit power(posit a, posit b) {
    return fromDouble(pow(toDouble(a), toDouble(b)));
}

posit negate(posit a) {
    if (is_neg(a)) {
        posit res = {(~a.bits) + 1};
        return res;
    }
    posit res = {~(a.bits - 1)};
    return res;
}

posit p_abs(posit ap) {
    posit_information a = fromPosit(ap);
    return a.neg ? negate(ap) : ap;
}

bool is_less(posit rhs, posit lhs) {
    return toDouble(rhs) < toDouble(lhs);
}
