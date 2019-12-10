#include "posit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define first_bit ((uint32_t)0x80000000)

struct posit add_posits(struct posit left, struct posit right) {
    // adding 1 to the beginning of fraction
    uint64_t left_frac = first_bit | (left.fraction >> 1);
    uint64_t right_frac = first_bit | (right.fraction >> 1);
    uint64_t frac;
    struct posit result;

    // need to choose the biggest exponent and also shift smallest fraction
    if (right.exponent > left.exponent) {
        result.exponent = right.exponent;
        left_frac = left_frac >> (right.exponent - left.exponent);
    } else {
        result.exponent = left.exponent;
        right_frac = right_frac >> (left.exponent - right.exponent);
    }
    frac = left_frac + right_frac;

    // dealing with an extra 1 which may occur
    if ((frac >> 32) != 0) {
        result.exponent++;
        frac = frac >> 1;
    }

    // removing the leftmost 1
    result.fraction = frac << 1;
    return result;
}

struct posit subtract_posits(struct posit left, struct posit right) {
    //again adding 1 to the beginning of fraction
    uint64_t left_frac = first_bit | (left.fraction >> 1);
    uint64_t right_frac = first_bit | (right.fraction >> 1);
    uint64_t frac;
    struct posit result;

    // again choosing the biggest exponent or biggest fraction if exponents are the same
    // and shifting the smallest fraction
    if (right.exponent > left.exponent || (right.exponent == left.exponent && right.fraction > left.fraction)) {
        result.exponent = right.exponent;
        left_frac = left_frac >> (right.exponent - left.exponent);
        frac = right_frac - left_frac;
    } else {
        result.exponent = left.exponent;
        right_frac = right_frac >> (left.exponent - right.exponent);
        frac = left_frac - right_frac;
    }

    result.exponent -= __builtin_clz(frac);
    result.fraction = frac << (__builtin_clz(frac) + 1);

    if (left.exponent == right.exponent && result.fraction == 0) {
        result = to_posit(0.0);
    }

    return result;
}

struct posit summa_posits(struct posit left, struct posit right) {
    struct posit result;

    // if any number is 0 -> returning another one
    if (left.exponent == -1054 && left.fraction == 0.0) return right;
    if (right.exponent == -1054 && right.fraction == 0.0) return left;

    // if signs are the same -> adding posits
    if (left.sign == right.sign) {
        result = add_posits(left, right);
        result.sign = left.sign;
    // else subtracting one from another
    } else if (left.sign > right.sign) {
        result = subtract_posits(right, left);
        if (fabs(to_double(right)) <= fabs(to_double(left))) {
            result.sign = left.sign;
        } else result.sign = right.sign;
    } else {
        result = subtract_posits(left, right);
        if (fabs(to_double(left)) >= fabs(to_double(right))) {
            result.sign = left.sign;
        } else result.sign = right.sign;
    }

    return result;
}

struct posit multiply_posits(struct posit left, struct posit right) {
    struct posit result;

    uint64_t left_frac = first_bit | (left.fraction >> 1);
    uint64_t right_frac = first_bit | (right.fraction >> 1);
    uint32_t frac = (left_frac * right_frac) >> 32;
    int32_t exp = left.exponent + right.exponent + 1;

    if ((frac & first_bit) == 0) {
        exp--;
        frac = frac << 1;
    }

    result.sign = left.sign ^ right.sign;
    result.exponent = exp;
    result.fraction = frac << 1;
    return result;
}

struct posit dot_product(size_t dim, struct posit *left, struct posit *right) {
    struct posit product[dim];
    struct posit temp;
    
    for (size_t i = 0; i < dim; i++) {
        product[i] = multiply_posits(left[i], right[i]);
    }
    
    for (size_t i = 0; i < dim; i++) {
        int32_t min_exp_diff = INT32_MAX;
        uint32_t min_frac_diff = UINT32_MAX;
        for (size_t j = i + 1; j < dim; j++) {
            int32_t exp_diff = abs(product[i].exponent - product[j].exponent);
            uint32_t frac_diff = product[i].fraction - product[j].fraction;
            if (exp_diff <= min_exp_diff) {
                if (exp_diff == min_exp_diff && min_frac_diff < frac_diff) continue;
                min_exp_diff = exp_diff;
                min_frac_diff = frac_diff;
                temp = product[i + 1];
                product[i + 1] = product[j];
                product[j] = temp;
            }
        }
    }
    for (size_t i = 0; i < dim; i++) {
        temp = product[i + 1];
        product[i + 1] = summa_posits(product[i], temp);
    }
    return product[5];
}

struct posit to_posit(double number) {
    union {
        double double_part;
        uint64_t uint_part;
    } converted_number;
    struct posit result;

    converted_number.double_part = number;
    result.sign = converted_number.uint_part >> 63;
    result.exponent = ((converted_number.uint_part >> 52) & 0x7FF) - 1023;
    result.fraction = (converted_number.uint_part << 12) >> 32;
    if (result.exponent == -1023) {
        result.exponent -= __builtin_clz(result.fraction);
        result.fraction = result.fraction << (__builtin_clz(result.fraction) + 1);
    }
    return result;
}

double to_double(struct posit number) {
    uint64_t exp_bits;
    uint64_t frac_bits;
    int exp = number.exponent + 1023;
    union {
        double double_part;
        uint64_t uint_part;
    } converted_number;

    if (exp < 1) {
        exp_bits = 0;
        frac_bits = (uint64_t)(first_bit | (number.fraction >> 1)) << (32);
        frac_bits = frac_bits >> -exp_bits;
    }
    else if (exp > 2046) {
        exp_bits = (uint64_t) 2046 << 53;
        frac_bits = -1;
    }
    else {
        exp_bits = (uint64_t)(exp & 0x7FF) << 53;
        frac_bits = (uint64_t)number.fraction << 32;
    }

    converted_number.uint_part = frac_bits;
    converted_number.uint_part = exp_bits | (converted_number.uint_part >> 11);
    converted_number.uint_part = ((uint64_t) number.sign << 63) | (converted_number.uint_part >> 1);
    return converted_number.double_part;
}
