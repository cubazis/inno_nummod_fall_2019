#include "posit.h"
#include <math.h>
#include <stdio.h>

#define LEFT_SHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))
#define RIGHT_SHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))
#define POW_2(n) (LEFT_SHIFT(1, (n)))
#define HIDDEN_BIT(a) ((a >> 1) | MSB)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define uint64 unsigned long long
#define uint32 unsigned int
#define bool short

#define FLOOR_DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define HIDDEN_BIT(a) ((a >> 1) | MSB)

const uint32 MSB = 1u << 31u;
const uint32 MASK = 0xFFFFFFFF;
const int ES = 2;
const int POSIT_SIZE = 32;
const int MAX_EXP = 120; // 2^ES * (POSIT_SIZE - 2)

typedef struct posit_info_t {
    bool neg;
    int exp;
    uint32 frac;
} posit_info;

bool _is_nar(posit p) {
    return p.bits == MSB;
}

bool _is_neg(posit p) {
    return (int) p.bits < 0 && !_is_nar(p);
}

bool _is_zero(posit p) {
    return p.bits == 0;
}

posit_info _from_double(double f) {
    posit_info up;
    int bias = 1023;

    union {
        double f;
        uint64 u;
    } un;

    un.f = f;
    up.neg = (bool) RIGHT_SHIFT(un.u, 63u);
    up.exp = (int) ((RIGHT_SHIFT(un.u, 52u)) & 0x7FFu) - bias;
    up.frac = (uint32) (RIGHT_SHIFT(LEFT_SHIFT(un.u, 12u), 32u));

    if (up.exp == -bias) {
        // normalize
        up.exp -= CLZ(up.frac);
        up.frac = LEFT_SHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

posit _to_posit(posit_info up) {

    if (up.exp < -MAX_EXP) {
        up.exp = -MAX_EXP;
    } else if (up.exp > MAX_EXP) {
        up.exp = MAX_EXP;
    }

    int reg = FLOOR_DIV(up.exp, POW_2(ES));
    uint32 rs = MAX(-reg + 1, reg + 2);
    uint32 exp = up.exp - POW_2(ES) * reg;

    uint32 regbits;
    if (reg < 0) {
        regbits = RIGHT_SHIFT(MSB, -reg);
    } else {
        regbits = LEFT_SHIFT(MASK, POSIT_SIZE - reg - 1);
    }
    uint32 expbits = LEFT_SHIFT(exp, POSIT_SIZE - ES);


    uint32 tmp = RIGHT_SHIFT(regbits, 1) | RIGHT_SHIFT(expbits, 1 + rs) | RIGHT_SHIFT(up.frac, 1 + rs + ES);

    if (up.neg) {
        tmp = ~(tmp - 1);
    }

    posit res= {tmp};
    return res;
}

posit from_double(double f) {
    return _to_posit(_from_double(f));
}

posit_info _from_posit(posit p) {

    posit_info res;
    res.neg = _is_neg(p);

    if (res.neg) {
        p.bits = (~p.bits) + 1;
    }

    int lz = CLZ(LEFT_SHIFT(p.bits, 1));
    int lo = CLZ(LEFT_SHIFT(~p.bits, 1) | 1);
    int rs = MIN(MAX(lz, lo) + 1, POSIT_SIZE - 1);

    int reg = (lz == 0 ? lo - 1 : -lz);
    int exp = RIGHT_SHIFT(LEFT_SHIFT(p.bits, 1 + rs), POSIT_SIZE - ES);

    res.exp = POW_2(ES) * reg + exp;
    res.frac = LEFT_SHIFT(p.bits, 1 + rs + ES);

    return res;
}

double to_double(posit a) {
    posit_info up = _from_posit(a);

    int fexp = up.exp + 1023;

    uint64 fexpbits;
    uint64 ffracbits;

    if (fexp > 2046) {
        fexpbits = LEFT_SHIFT((uint64)2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = LEFT_SHIFT((uint64)(MSB | RIGHT_SHIFT(up.frac, 1)), 64 - POSIT_SIZE);
        ffracbits = RIGHT_SHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LEFT_SHIFT((uint64)(fexp & 0x7FF), 53);
        ffracbits = LEFT_SHIFT((uint64)up.frac, 64 - POSIT_SIZE);
    }

    union {
        double f;
        uint64 u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RIGHT_SHIFT(un.u, 11);
    un.u = LEFT_SHIFT((uint64)up.neg, 63) | RIGHT_SHIFT(un.u, 1);

    if (LEFT_SHIFT(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

posit_info _subtract(posit_info a, posit_info b, bool neg) {
    posit_info r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    uint64 frac;

    if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
        r.exp = a.exp;
        bfrac = RIGHT_SHIFT(bfrac, a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        neg = !neg;
        r.exp = b.exp;
        afrac = RIGHT_SHIFT(afrac, b.exp - a.exp);
        frac = bfrac - afrac;
    }

    r.neg = neg;
    r.exp -= CLZ(frac);
    r.frac = LEFT_SHIFT(frac, CLZ(frac) + 1);

    return r;
}

posit_info _add(posit_info a, posit_info b, bool neg) {
    posit_info r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    uint64 frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = RIGHT_SHIFT(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = RIGHT_SHIFT(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (RIGHT_SHIFT(frac, POSIT_SIZE) != 0) {
        r.exp++;
        frac = RIGHT_SHIFT(frac, 1);
    }

    r.neg = neg;
    r.frac = LEFT_SHIFT(frac, 1);

    return r;
}

posit add(posit ap, posit bp) {
    posit_info a = _from_posit(ap);
    posit_info b = _from_posit(bp);

    if (_is_opposite(a, b, 0))
        return from_double(0);

    if (a.neg == b.neg) {
        return _to_posit(_add(a, b, a.neg));
    } else {
        return _to_posit(_subtract(a, b, a.neg));
    }
}

posit subtract(posit ap, posit bp) {
    posit_info a = _from_posit(ap);
    posit_info b = _from_posit(bp);

    if (a.neg == b.neg) {
        return _to_posit(_subtract(a, b, a.neg));
    } else {
        return _to_posit(_add(a, b, a.neg));
    }
}

bool _is_opposite(posit_info a, posit_info b, bool neg) {
    if (a.exp == b.exp && a.frac == b.frac)
        if ((neg && (a.neg == b.neg)) || (!neg && (a.neg != b.neg)) )
            return 1;
    return 0;
}

posit divide(posit ap, posit bp) {
    posit_info a = _from_posit(ap);
    posit_info b = _from_posit(bp);
    posit_info r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    int exp = a.exp - b.exp;

    if (afrac < bfrac) {
        exp--;
        bfrac = RIGHT_SHIFT(bfrac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LEFT_SHIFT(afrac, POSIT_SIZE) / bfrac;

    return _to_posit(r);
}

posit multiply(posit ap, posit bp) {
    posit_info a = _from_posit(ap);
    posit_info b = _from_posit(bp);
    posit_info r;

    uint64 afrac = HIDDEN_BIT(a.frac);
    uint64 bfrac = HIDDEN_BIT(b.frac);
    uint32 frac = RIGHT_SHIFT(afrac * bfrac, POSIT_SIZE);
    int exp = a.exp + b.exp + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = LEFT_SHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LEFT_SHIFT(frac, 1);

    return _to_posit(r);
}

posit negate(posit a) {
    if (_is_neg(a)) {
        posit res = {(~a.bits) + 1};
        return res;
    }
    posit res = {~(a.bits - 1)};
    return res;
}

posit power(posit a, posit b) {
    return from_double(pow(to_double(a), to_double(b)));
}

int test_function() {
    return 42;
}
