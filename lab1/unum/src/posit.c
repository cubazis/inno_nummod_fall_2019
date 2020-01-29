//
// Created by cubazis on 18.08.2019.
//

#include "posit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define MSB ((uint32_t)0x80000000)

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

    // size occupied in memory - 64 bit
    union {
        double d;
        uint64_t u;
    } memory_unit;

    memory_unit.d = f;
    result.sign = memory_unit.u >> 63;
    result.exponent = ((memory_unit.u >> 52) & 0x7FF) - bias;
    result.fraction = (memory_unit.u << 12) >> 32;

    if (result.exponent == -bias) {
        result.exponent -= __builtin_clz(result.fraction);
        result.fraction = result.fraction << (__builtin_clz(result.fraction) + 1);
    }
    return result;
}

struct posit32 add(struct posit32 a, struct posit32 b) {
    struct posit32 res;

    // Берем fraction и добавляем 1 перед ним (так как он на самом деле 1 + frac / 2^es)
    uint64_t a_frac = MSB | (a.fraction >> 1);
    uint64_t b_frac = MSB | (b.fraction >> 1);
    uint64_t frac;

    if (a.exponent > b.exponent) { // берем большую экспоненту числа
        res.exponent = a.exponent;
        b_frac = b_frac >> (a.exponent - b.exponent); // сдвигаем fraction меньшего
    } else {
        res.exponent = b.exponent;
        a_frac = a_frac >> (b.exponent - a.exponent);
    }
    frac = a_frac + b_frac;

    // выше мог получиться лишний бит слева
    // В этом случае увеличиваем exp (и regime если экспонента переполнилась) нового числа,
    // а сам fraction сдвигаем вправо на 1
    if ((frac >> 32) != 0) {
        res.exponent++;
        frac = frac >> 1;
    }

    res.fraction = frac << 1; // Отсекаем единицу
    return res;
}

struct posit32 subtract(struct posit32 a, struct posit32 b) {
    struct posit32 result;

    // Берем fraction и добавляем 1 перед ним (так как он на самом деле 1 + frac / 2^es)
    uint64_t a_frac = MSB | (a.fraction >> 1);
    uint64_t b_frac = MSB | (b.fraction >> 1);
    uint64_t frac;

    if (a.exponent > b.exponent || (a.exponent == b.exponent && a.fraction > b.fraction)) {
        result.exponent = a.exponent;
        b_frac = b_frac >> (a.exponent - b.exponent);
        frac = a_frac - b_frac;
    } else {
        result.exponent = b.exponent;
        a_frac = a_frac >> (b.exponent - a.exponent);
        frac = b_frac - a_frac;
    }
    result.exponent -= __builtin_clz(frac);  // counting leading zeroes
    result.fraction = frac << (__builtin_clz(frac) + 1);  // чекаем переполнение нового fraction за 0
    if (a.exponent == b.exponent && result.fraction == 0) {
        result = to_posit32(0.0);
    }
    return result;
}

struct posit32 sum(struct posit32 a, struct posit32 b) {
    struct posit32 result;

    // checking for 0 addition in the beginning for efficiency
    if (a.exponent == -1054 && a.fraction == 0.0) return b;
    if (b.exponent == -1054 && b.fraction == 0.0) return a;

    // depending on sign it might be either subtraction or addition
    if (a.sign == b.sign) {
        result = add(a, b);
        result.sign = a.sign;
    } else if (a.sign > b.sign) {
        result = subtract(b, a);
        if (fabs(to_double(b)) > fabs(to_double(a))) {
            result.sign = b.sign;
        } else result.sign = a.sign;
    } else {
        result = subtract(a, b);
        if (fabs(to_double(a)) >= fabs(to_double(b))) {
            result.sign = a.sign;
        } else result.sign = b.sign;
    }
    return result;
}

struct posit32 multiply(struct posit32 a, struct posit32 b) {
    struct posit32 result;

    uint64_t a_frac = MSB | (a.fraction >> 1);
    uint64_t b_frac = MSB | (b.fraction >> 1);
    uint32_t frac = (a_frac * b_frac) >> 32;
    int32_t exp = a.exponent + b.exponent + 1;

    if ((frac & MSB) == 0) {
        exp--;
        frac = frac << 1;
    }

    result.sign = a.sign ^ b.sign;
    result.exponent = exp;
    result.fraction = frac << 1;
    return result;
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
            int32_t exp_diff = abs(multiplications[i].exponent - multiplications[j].exponent);
            uint32_t frac_diff = multiplications[i].fraction - multiplications[j].fraction;
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
