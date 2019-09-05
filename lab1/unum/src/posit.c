//
// 32 bit posit implementation with es = 2
//

#include "posit.h"
#include <stddef.h>
#include <stdio.h>

int test_function() {
    posit a = pack_posit(unpack_double(0.70));
    posit b = pack_posit(unpack_double(0.05));
    posit p = add(a, b);
    double d = pack_double(unpack_posit(p));
    printf("%u, %.12f, %.12f\n", p.v, d, (float)0.75);
    return 42;
}

posit pack_posit(unpacked_posit up) {
    POSIT_UTYPE p;
    POSIT_UTYPE regbits;
    POSIT_UTYPE expbits;

    int maxexp = POW2(ES) * (NBITS - 2);
    if (up.exp < -maxexp)
        up.exp = -maxexp;
    else if (up.exp > maxexp)
        up.exp = maxexp;

    int reg = FLOORDIV(up.exp, POW2(ES));
    int ss = get_ss();
    int rs = MAX(-reg + 1, reg + 2);

    if (ss + rs + ES >= NBITS && up.frac >= POSIT_MSB) {
        up.exp++;
        reg = FLOORDIV(up.exp, POW2(ES));
        rs = MAX(-reg + 1, reg + 2);
    }

    POSIT_STYPE exp = up.exp - POW2(ES) * reg;

    regbits = reg < 0 ? RSHIFT(POSIT_MSB, -reg) : LMASK(POSIT_MASK, reg + 1);
    expbits = LMASK(LSHIFT(exp, NBITS - ES), ES);

    p = up.frac;
    p = expbits | RSHIFT(p, ES);
    p = regbits | RSHIFT(p, rs);
    p = RSHIFT(p, ss);

    posit res = { .v = up.neg ? get_neg(p) : LMASK(p, NBITS)};
    return res;
}

double pack_double(unpacked_posit up) {
    int fexp = up.exp + 1023;

    // left aligned
    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        // overflow, set maximum value
        fexpbits = LSHIFT((uint64_t)2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        // underflow, pack as denormal
        fexpbits = 0;
        ffracbits = LSHIFT((uint64_t)(POSIT_MSB | RSHIFT(up.frac, 1)), 64 - NBITS);
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((uint64_t)(fexp & 0x7FF), 53);
        ffracbits = LSHIFT((uint64_t)up.frac, 64 - NBITS);
    }

    union {
        double f;
        uint64_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((uint64_t)up.neg, 63) | RSHIFT(un.u, 1);

    // don't underflow to zero
    if (LSHIFT(un.u, 1) == 0) un.u++;

    return un.f;
}

unpacked_posit unpack_double(double d) {
    unpacked_posit up;
    int bias = 1023;

    union {
        double d;
        uint64_t u;
    } un;

    un.d = d;
    up.neg  = (bool)RSHIFT(un.u, 63);
    up.exp  = (POSIT_STYPE)((RSHIFT(un.u, 52) & 0x7FF) - bias);
    up.frac = (POSIT_UTYPE)(RSHIFT(LSHIFT(un.u, 12), 64 - NBITS));

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

unpacked_posit unpack_posit(posit p) {
    unpacked_posit up;

    bool neg = is_neg(p);
    if (neg) {
        p.v = get_neg(p.v);
    }

    int ss = get_ss();
    int rs = get_rs(p);

    int lz = CLZ(LSHIFT(p.v, ss));
    int lo = CLZ(LSHIFT(~p.v, ss) | 1); // add LSB to compensate for sign bit

    int reg = (lz == 0 ? lo - 1 : -lz);
    POSIT_UTYPE exp = RSHIFT(LSHIFT(p.v, ss + rs), NBITS - ES);

    up.neg = neg;
    up.exp = POW2(ES) * reg + exp;
    up.frac = LSHIFT(p.v, ss + rs + ES);

    return up;
}

posit add(posit rhs, posit lhs) {
    unpacked_posit a = unpack_posit(rhs);
    unpacked_posit b = unpack_posit(lhs);

    if (same_opposite(a, b, 1)) return (posit){ .v = POSIT_ZERO };

    unpacked_posit r;
    if (a.neg == b.neg) r = add_(a, b, a.neg);
    else                r = sub_(a, b, a.neg);

    return pack_posit(r);
}

posit sub(posit rhs, posit lhs) {
    unpacked_posit a = unpack_posit(rhs);
    unpacked_posit b = unpack_posit(lhs);

    if (same_opposite(a, b, 0)) return (posit){ .v = POSIT_ZERO };

    unpacked_posit r;
    if (a.neg == b.neg) r = sub_(a, b, a.neg);
    else                r = add_(a, b, a.neg);

    return pack_posit(r);
}

unpacked_posit add_(unpacked_posit a, unpacked_posit b, bool neg) {
    // TODO: 0.70 + 0.05 gives a little bit less (1)
    unpacked_posit r;

    POSIT_LUTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_LUTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_LUTYPE frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (RSHIFT(frac, NBITS) != 0) {
        r.exp++;
        frac = RSHIFT(frac, 1);
    }

    r.neg = neg;
    r.frac = LSHIFT(frac, 1);

    return r;
}

unpacked_posit sub_(unpacked_posit a, unpacked_posit b, bool neg) {
    unpacked_posit r;

    POSIT_UTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_UTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_UTYPE frac;

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

posit mult(posit rhs, posit lhs) {
    unpacked_posit a = unpack_posit(rhs);
    unpacked_posit b = unpack_posit(lhs);

    unpacked_posit r;

    POSIT_LUTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_LUTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_UTYPE frac = RSHIFT(afrac * bfrac, NBITS);
    POSIT_STYPE exp = a.exp + b.exp + 1;

    if ((frac & POSIT_MSB) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return pack_posit(r);
}

posit p_pow(posit a, posit b) {
    posit r;
    int n = (int)pack_double(unpack_posit(b));

    if (is_zero(b) || n == 0) {
        r.v = POSIT_ONE;
        return r;
    }

    r = a;
    for (int i = 1; i < n; i++)
        r = mult(r, a);

    return r;
}

posit p_abs(posit p) {
    unpacked_posit up = unpack_posit(p);
    return up.neg ? get_neg_p(p) : p;
}

bool is_less(posit rhs, posit lhs) {
    double a = pack_double(unpack_posit(rhs));
    double b = pack_double(unpack_posit(lhs));
    return a < b;
}

bool same_opposite(unpacked_posit a, unpacked_posit b, bool is_add) {
    if (a.exp == b.exp && a.frac == b.frac)
        if ((is_add && (a.neg != b.neg)) || (!is_add && (a.neg == b.neg)))
            return 1;
    return 0;
}

posit make_posit(double d) { return pack_posit(unpack_double(d)); }

bool is_zero(posit p) { return p.v == POSIT_ZERO; }

bool is_nar(posit p)  { return p.v == POSIT_NAR; }

bool is_neg(posit p) { return (POSIT_STYPE)p.v < 0 && !is_nar(p); }

int get_ss() { return 1; }

int get_rs(posit p) {
    int ss = get_ss();
    int lz = CLZ(LSHIFT(p.v, ss));
    int lo = CLZ(LSHIFT(~p.v, ss));
    int rs = MAX(lz, lo) + 1;
    return MIN(rs, NBITS - ss);
}

int get_es(posit p) {
    int ss = get_ss();
    int rs = get_rs(p);
    return MIN(MAX(NBITS - ss - rs, 0), ES);
}

int get_fs(posit p) {
    int ss = get_ss();
    int rs = get_rs(p);
    return MAX(NBITS - ss - rs - ES, 0);
}

posit get_neg_p(posit p) {
    p.v = get_neg(p.v);
    return p;
}

POSIT_UTYPE get_neg(POSIT_STYPE p) {
    return LMASK(-LMASK(p, NBITS), NBITS);
}