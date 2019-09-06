#include "posit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


double to_double(struct posit32 p) {
    int fexp = p.exponent + 1023;
    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        fexpbits = (uint64_t) 2046 << 53;
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = (uint64_t) (MSB | (p.fraction >> 1)) << (32);
        ffracbits = ffracbits >> -fexpbits;
    } else {
        fexpbits = (uint64_t) (fexp & 0x7FF) << 53;
        ffracbits = (uint64_t) p.fraction << 32;
    }

    union {
        double d;
        uint64_t u;
    } memory_unit;

    memory_unit.u = ffracbits;
    memory_unit.u = fexpbits | (memory_unit.u >> 11);
    memory_unit.u = ((uint64_t) p.sign << 63) | (memory_unit.u >> 1);
    return memory_unit.d;
}

struct posit32 to_posit32(double f) {
    struct posit32 result;
    int bias = 1023;
    union {
        double d;
        uint64_t u;
    } memory_unit;
    memory_unit.d = f;
    result.sign = memory_unit.u >> 63;
    result.exponent = ((memory_unit.u >> 52) & 0x7FF) - bias;
    result.fraction = (memory_unit.u << 12) >> 32;
    if (result.exponent == -bias) {
        result.exponent -= CLZ(result.fraction);
        result.fraction = result.fraction << (CLZ(result.fraction) + 1);
    }
    return result;
}

struct posit32 add(struct posit32 p1, struct posit32 p2) {
    struct posit32 result;
    uint64_t p1_frac = MSB | (p1.fraction >> 1);
    uint64_t p2_frac = MSB | (p2.fraction >> 1);
    uint64_t frac;

    if (p1.exponent > p2.exponent) {
        result.exponent = p1.exponent;
        p2_frac = p2_frac >> (p1.exponent - p2.exponent);
    } else {
        result.exponent = p2.exponent;
        p1_frac = p1_frac >> (p2.exponent - p1.exponent);
    }
    frac = p1_frac + p2_frac;

    if ((frac >> 32) != 0) {
        result.exponent++;
        frac = frac >> 1;
    }

    result.fraction = frac << 1;
    return result;
}

struct posit32 subtract(struct posit32 p1, struct posit32 p2) {
    struct posit32 result;

    uint64_t p1_frac = MSB | (p1.fraction >> 1);
    uint64_t p2_frac = MSB | (p2.fraction >> 1);
    uint64_t frac;

    if (p1.exponent > p2.exponent || (p1.exponent == p2.exponent && p1.fraction > p2.fraction)) {
        result.exponent = p1.exponent;
        p2_frac = p2_frac >> (p1.exponent - p2.exponent);
        frac = p1_frac - p2_frac;
    } else {
        result.exponent = p2.exponent;
        p1_frac = p1_frac >> (p2.exponent - p1.exponent);
        frac = p2_frac - p1_frac;
    }
    result.exponent -= CLZ(frac);
    result.fraction = frac << (CLZ(frac) + 1);
    if (p1.exponent == p2.exponent && result.fraction == 0) {
        result = to_posit32(0.0);
    }
    return result;
}

struct posit32 sum(struct posit32 p1, struct posit32 p2) {
    struct posit32 result;
    if (p1.exponent == -1054 && p1.fraction == 0.0) return p2;
    if (p2.exponent == -1054 && p2.fraction == 0.0) return p1;
    if (p1.sign == p2.sign) {
        result = add(p1, p2);
        result.sign = p1.sign;
    } else if (p1.sign > p2.sign) {
        result = subtract(p2, p1);
        if (fabs(to_double(p2)) > fabs(to_double(p1))) {
            result.sign = p2.sign;
        } else result.sign = p1.sign;
    } else {
        result = subtract(p1, p2);
        if (fabs(to_double(p1)) >= fabs(to_double(p2))) {
            result.sign = p1.sign;
        } else result.sign = p2.sign;
    }
    return result;
}

struct posit32 multiply(struct posit32 p1, struct posit32 p2) {
    struct posit32 result;

    uint64_t p1_frac = MSB | (p1.fraction >> 1);
    uint64_t p2_frac = MSB | (p2.fraction >> 1);
    uint32_t frac = (p1_frac * p2_frac) >> 32;
    int32_t exp = p1.exponent + p2.exponent + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = frac << 1;
    }

    result.sign = p1.sign ^ p2.sign;
    result.exponent = exp;
    result.fraction = frac << 1;
    return result;
}

int32_t exponent_differ(struct posit32 p1, struct posit32 p2) {
    return abs(p1.exponent - p2.exponent);
}

uint64_t fraction_differ(struct posit32 p1, struct posit32 p2) {
    return (p1.fraction - p2.fraction);
}

struct posit32 dot_product(size_t dim, struct posit32 *v1, struct posit32 *v2) {
    struct posit32 multiplications[dim];
    for (size_t i = 0; i < dim; i++) {
        multiplications[i] = multiply(v1[i], v2[i]);
    }
    struct posit32 temp;
    for (size_t i = 0; i < dim; i++) {
        int32_t min_exp_diff = INT32_MAX;
        uint32_t min_frac_diff = UINT32_MAX;
        for (size_t j = i + 1; j < dim; j++) {
            int32_t exp_diff = exponent_differ(multiplications[i], multiplications[j]);
            uint32_t frac_diff = fraction_differ(multiplications[i], multiplications[j]);
            if (exp_diff <= min_exp_diff) {
                if (exp_diff == min_exp_diff && min_frac_diff < frac_diff) continue;
                min_exp_diff = exp_diff;
                min_frac_diff = frac_diff;
                temp = multiplications[i + 1];
                multiplications[i + 1] = multiplications[j];
                multiplications[j] = temp;
            }
        }
    }
    for (size_t i = 0; i < dim; i++) {
        temp = multiplications[i + 1];
        multiplications[i + 1] = sum(multiplications[i], temp);
    }
    return multiplications[5];
}
