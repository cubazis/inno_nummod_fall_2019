#include <math.h>
#include <stdint.h>

#include "posit.h"

typedef struct unpacked_t {
    bool neg;
    int32_t exp;
    uint32_t frac;
} unpacked;

bool is_nar(posit p) {
    return p.bits == MSB;
}

bool is_neg(posit p) {
    return (int) p.bits < 0 && !is_nar(p);
}

int32_t regime_size(posit p) {
    int32_t lz = CLZ(LSHIFT(p.bits, 1));
    int32_t lo = CLZ(LSHIFT(~p.bits, 1));
    int32_t rs = MAX(lz, lo) + 1;

    return MIN(rs, POSIT_SIZE - 1);
}

unpacked double_to_unpacked(double f) {
    unpacked up;
    int32_t bias = 1023;

    union {
        double f;
        uint64_t  u;
    } un;

    un.f = f;
    up.neg = (bool) RSHIFT(un.u, 63u);
    up.exp = (int) ((RSHIFT(un.u, 52u)) & 0x7FFu) - bias;
    up.frac = (uint32_t) (RSHIFT(LSHIFT(un.u, 12u), 32u));

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

posit unpacked_to_posit(unpacked up) {
    if (up.exp < -MAX_EXP)
        up.exp = -MAX_EXP;
    else if (up.exp > MAX_EXP)
        up.exp = MAX_EXP;

    int32_t reg = up.exp / POW2(ES) - (up.exp % POW2(ES) < 0);

    uint32_t ss = 1;
    uint32_t rs = MAX(-reg + 1, reg + 2);
    uint32_t exp = up.exp - POW2(ES) * reg;

    uint32_t regbits;
    if (reg < 0)
        regbits = RSHIFT(MSB, -reg);
    else
        regbits = LSHIFT(MASK, POSIT_SIZE - reg - 1);
    uint32_t expbits = LSHIFT(exp, POSIT_SIZE - ES);


    uint32_t tmp = RSHIFT(regbits, ss)
                   | RSHIFT(expbits, ss + rs)
                   | RSHIFT(up.frac, ss + rs + ES);

    if (up.neg)
        tmp = ~(tmp - 1);

    posit res = {tmp};
    return res;
}

posit double_to_posit(double f) {
    return unpacked_to_posit(double_to_unpacked(f));
}

unpacked posit_to_unpacked(posit p) {
    unpacked res;
    res.neg = is_neg(p);

    if (res.neg)
        p.bits = (~p.bits) + 1;

    int32_t rs = regime_size(p);

    int32_t lz = CLZ(LSHIFT(p.bits, 1));
    int32_t lo = CLZ(LSHIFT(~p.bits, 1) | 1);

    int32_t reg = (lz == 0 ? lo - 1 : -lz);
    int32_t exp = RSHIFT(LSHIFT(p.bits, 1 + rs), POSIT_SIZE - ES);

    res.exp = POW2(ES) * reg + exp;
    res.frac = LSHIFT(p.bits, 1 + rs + ES);

    return res;
}

double posit_to_double(posit a) {
    unpacked up = posit_to_unpacked(a);

    int32_t fexp = up.exp + 1023;

    uint64_t  fexpbits;
    uint64_t  ffracbits;

    if (fexp > 2046) {
        fexpbits = LSHIFT((uint64_t ) 2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = LSHIFT((uint64_t )(MSB | RSHIFT(up.frac, 1)), 64 - POSIT_SIZE);
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((uint64_t )(fexp & 0x7FF), 53);
        ffracbits = LSHIFT((uint64_t ) up.frac, 64 - POSIT_SIZE);
    }

    union {
        double f;
        uint64_t  u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((uint64_t ) up.neg, 63) | RSHIFT(un.u, 1);

    if (LSHIFT(un.u, 1) == 0)
        un.u++;

    return un.f;
}

unpacked _add(unpacked a, unpacked b, bool neg) {
    unpacked r;

    uint64_t  afrac = HIDDEN_BIT(a.frac);
    uint64_t  bfrac = HIDDEN_BIT(b.frac);
    uint64_t  frac;

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

unpacked _sub(unpacked a, unpacked b, bool neg) {
    unpacked r;

    uint64_t  afrac = HIDDEN_BIT(a.frac);
    uint64_t  bfrac = HIDDEN_BIT(b.frac);
    uint64_t  frac;

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

bool _opposite(unpacked a, unpacked b, bool neg) {
    if (a.exp == b.exp && a.frac == b.frac)
        return (a.neg + b.neg + neg) % 2;
    return 0;
}

posit add(posit ap, posit bp) {
    unpacked a = posit_to_unpacked(ap);
    unpacked b = posit_to_unpacked(bp);

    if (_opposite(a, b, 0))
        return double_to_posit(0);
    else if (a.neg == b.neg)
        return unpacked_to_posit(_add(a, b, a.neg));
    else
        return unpacked_to_posit(_sub(a, b, a.neg));
}

posit sub(posit ap, posit bp) {
    unpacked a = posit_to_unpacked(ap);
    unpacked b = posit_to_unpacked(bp);

    if (_opposite(a, b, 1))
        return double_to_posit(0);
    else if (a.neg == b.neg)
        return unpacked_to_posit(_sub(a, b, a.neg));
    else
        return unpacked_to_posit(_add(a, b, a.neg));
}

posit mul(posit ap, posit bp) {
    unpacked a = posit_to_unpacked(ap);
    unpacked b = posit_to_unpacked(bp);
    unpacked r;

    uint64_t  afrac = HIDDEN_BIT(a.frac);
    uint64_t  bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac = RSHIFT(afrac * bfrac, POSIT_SIZE);
    int32_t exp = a.exp + b.exp + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return unpacked_to_posit(r);
}


posit power(posit a, posit b) {
    return double_to_posit(pow(posit_to_double(a), posit_to_double(b)));
}

posit negate(posit a) {
    if (is_neg(a)) {
        posit res = {(~a.bits) + 1};
        return res;
    }
    posit res = {~(a.bits - 1)};
    return res;
}