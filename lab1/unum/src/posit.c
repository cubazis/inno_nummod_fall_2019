
#include "posit.h"
#include <stddef.h>
#include <stdio.h>

int test_function() {
    posit a = get_posit_from_double(0.90);
    posit b = get_posit_from_double(0.05);
    posit p = add(a, b);
    double d = get_double(p);
    printf("%u, %.12f, %.12f\n", p.v, d, (float) 0.95);
    return 42;
}

uint32_t get_posit_from_bits(bool neg, int32_t exp, uint32_t frac) {
    uint32_t p;
    uint32_t regbits;
    uint32_t expbits;

    int maxexp = (1 << ES) * (NBITS - 2);
    if (exp < -maxexp)
        exp = -maxexp;
    else if (exp > maxexp)
        exp = maxexp;

    int reg = FLOORDIV(exp, 1 << ES);
    int ss = get_sign_size();
    int rs = MAX(-reg + 1, reg + 2);

    if (ss + rs + ES >= NBITS && frac >= MSB) {
        exp++;
        reg = FLOORDIV(exp, 1 << ES);
        rs = MAX(-reg + 1, reg + 2);
    }

    int32_t exponent = exp - (1 << ES) * reg;

    regbits = reg < 0 ? MSB >> (-reg) : MASK << (reg + 1);
    expbits = LMASK(exponent << (NBITS - ES), ES);

    p = frac;
    p = expbits | (p >> ES);
    p = regbits | (p >> rs);
    p = p >> ss;

    uint32_t result = {neg ? get_neg(p) : LMASK(p, NBITS)};
    return result;
}

double get_double(posit p) {
    int fexp = p.exp + 1023;

    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        fexpbits = (uint64_t) 2046 << 53;
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        if (64 - NBITS >= (8 * sizeof(MSB | (p.frac >> 1)))) {
            ffracbits = 0;
        } else
            ffracbits = (uint64_t) ((MSB | (p.frac >> 1)) << (64 - NBITS));
        ffracbits = ffracbits >> -fexp;
    } else {
        fexpbits = (uint64_t) (fexp & 0x7FF) << 53;
        ffracbits = (uint64_t) p.frac << (64 - NBITS);
    }

    union {
        double f;
        uint64_t u;
    } d;

    d.u = ffracbits;
    d.u = fexpbits | (d.u >> 11);
    d.u = ((uint64_t) p.neg << 63) | (d.u >> 1);

    if ((d.u << 1) == 0) d.u++;

    return d.f;
}

posit get_posit_from_double(double d) {
    posit p;
    int bias = 1023;

    union {
        double d;
        uint64_t u;
    } un;

    un.d = d;
    p.neg = (bool) (un.u >> 63);
    p.exp = (int32_t) ((un.u >> 52) & 0x7FF) - bias;
    p.frac = (uint32_t) ((un.u << 12) >> (64 - NBITS));

    if (p.exp == -bias) {
        p.exp -= CLZ(p.frac);
        p.frac = p.frac << (CLZ(p.frac) + 1);
    }
    p.v = get_posit_from_bits(p.neg, p.exp, p.frac);

    return p;
}

posit add(posit rhs, posit lhs) {

    if (same_opposite(rhs, lhs, 1)) return (posit) {.v = ZERO};

    posit result;
    if (rhs.neg == lhs.neg) result = add_(rhs, lhs, rhs.neg);
    else result = sub_(rhs, lhs, rhs.neg);
    result.v = get_posit_from_bits(result.neg, result.exp, result.frac);
    return result;
}

posit subtract(posit rhs, posit lhs) {
    if (same_opposite(rhs, lhs, 0)) return (posit) {.v = ZERO};

    posit result;
    if (rhs.neg == lhs.neg) result = sub_(rhs, lhs, rhs.neg);
    else result = add_(rhs, lhs, rhs.neg);
    result.v = get_posit_from_bits(result.neg, result.exp, result.frac);
    return result;
}

posit add_(posit a, posit b, bool neg) {
    posit result;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    uint64_t frac;

    if (a.exp > b.exp) {
        result.exp = a.exp;
        bfrac = bfrac >> (a.exp - b.exp);
    } else {
        result.exp = b.exp;
        afrac = afrac >> (b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if ((frac >> NBITS) != 0) {
        result.exp++;
        frac = frac >> 1;
    }

    result.neg = neg;
    result.frac = frac << 1;
    result.v = get_posit_from_bits(result.neg, result.exp, result.frac);

    return result;
}

posit sub_(posit a, posit b, bool neg) {
    posit result;

    uint32_t afrac = HIDDEN_BIT(a.frac);
    uint32_t bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac;

    if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
        result.exp = a.exp;
        bfrac = bfrac >> (a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        neg = !neg;
        result.exp = b.exp;
        afrac = afrac >> (b.exp - a.exp);
        frac = bfrac - afrac;
    }

    result.neg = neg;
    result.exp -= CLZ(frac);
    result.frac = frac << (CLZ(frac) + 1);
    result.v = get_posit_from_bits(result.neg, result.exp, result.frac);

    return result;
}

posit multiply(posit rhs, posit lhs) {

    posit result;

    uint64_t afrac = HIDDEN_BIT(rhs.frac);
    uint64_t bfrac = HIDDEN_BIT(lhs.frac);
    uint32_t frac = (afrac * bfrac) >> NBITS;
    int32_t exp = rhs.exp + lhs.exp + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = frac << 1;
    }

    result.neg = rhs.neg ^ lhs.neg;
    result.exp = exp;
    result.frac = frac << 1;
    result.v = get_posit_from_bits(result.neg, result.exp, result.frac);
    return result;
}

posit power(posit a, posit b) {
    posit result;
    int n = (int) get_double(b);

    if (is_zero(b) || n == 0) {
        result.v = ONE;
        return result;
    }

    result = a;
    for (int i = 1; i < n; i++)
        result = multiply(result, a);

    return result;
}

posit posit_abs(posit p) {
    return p.neg ? get_neg_p(p) : p;
}

bool is_less(posit rhs, posit lhs) {
    double a = get_double(rhs);
    double b = get_double(lhs);
    return a < b;
}

bool same_opposite(posit a, posit b, bool is_add) {
    if (a.exp == b.exp && a.frac == b.frac)
        if ((is_add && (a.neg != b.neg)) || (!is_add && (a.neg == b.neg)))
            return 1;
    return 0;
}

bool is_zero(posit p) { return p.v == ZERO; }

int get_sign_size() { return 1; }

posit get_neg_p(posit p) {
    p.v = get_neg(p.v);
    return p;
}

uint32_t get_neg(int32_t p) {
    return LMASK(-LMASK(p, NBITS), NBITS);
}