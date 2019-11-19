#include <stdint.h>
#include <stdbool.h>
#include "posit.h"

typedef struct {
    bool is_negative;
    uint32_t exp;
    uint32_t fraction;
} repr;

posit encode_posit(repr r) {
    posit result = 0;

    int exp_initial = r.exp & 0b111;
    int regime;
    int regime_shifts;

    if (exp_initial >= 0) {
        regime = (((uint32_t) exp_initial) >> 3) + 1;
        regime_shifts = regime;
        for (int i = 0; i < regime; i++) {
            result |= 1 << (30 - i);
        }
    } else {
        regime = ((uint32_t) -(exp_initial - r.exp)) >> 3;
        regime_shifts = regime;
        result |= 1 << (30 - regime);
    }

    result |= r.exp << (30 - regime_shifts - 3);
    result |= r.fraction >> (1 + regime_shifts + 4);

    if ((r.fraction >> (4 + regime_shifts)) & 1) {
        result += 1; // rounding
    }

    if (r.is_negative) {
        result = ~result + 1;
    }

    return result;
}

posit p32_from_double(double a) {
    if (a == 0.0) {
        return (posit) 0;
    }

    union {
        double input;
        uint64_t bits;
    } val;

    val.input = a;
    int exp = ((val.bits >> 52) & 0x7FF) - 1023;
    uint32_t fraction = (uint32_t) ((val.bits >> 20) & 0xFFFFFFFF); // fit fraction into 32 bit container
    return encode_posit((repr) {.is_negative=(a < 0), .exp=exp, .fraction=fraction});
}

posit p32_add(posit a, posit b) {
    posit result;

    if (a == 0) return b;
    if (b == 0) return a;

    uint32_t negative1 = (a & 0x80000000) >> 31;
    if (negative1) a = -a;

    uint32_t negative2 = (b & 0x80000000) >> 31;
    if (negative2) b = -b;

    int exp_a = 0;
    int exp_b = 0;

    // Decode `a`
    int first_regime_bit = (a >> 30) & 1;
    int shifts = 0;
    int shifts_a = 0;
    int shifts_b = 0;
    int current_bit = first_regime_bit;
    while (current_bit == first_regime_bit) {
        current_bit = (a >> (29 - shifts)) & 1;
        shifts++;
    }
    int exp = (a >> (30 - shifts - 3)) & 0b111;
    if (first_regime_bit == 1) {
        exp |= (shifts - 1) << 3;
    } else {
        exp |= (-(shifts << 3)) | exp;
    }
    if (a & 0x80000000) exp = -exp;
    exp_a = exp;
    shifts_a = shifts;

    // Decode `b`
    first_regime_bit = (b >> 30) & 1;
    shifts = 0;
    current_bit = first_regime_bit;
    while (current_bit == first_regime_bit) {
        current_bit = (b >> (29 - shifts)) & 1;
        shifts++;
    }
    exp = (b >> (30 - shifts - 3)) & 0b111;

    if (first_regime_bit == 1) {
        exp |= (shifts - 1) << 3;
    } else {
        exp |= (-(shifts << 3)) | exp;
    }
    if (b & 0x80000000) exp = -exp;
    exp_b = exp;
    shifts_b = shifts;

    // Add `a` and `b`
    int exp_diff = exp_a - exp_b;

    int fraction_a = a;
    fraction_a <<= 2 + shifts_a;

    int fraction_b = b;
    fraction_b <<= 2 + shifts_b;

    fraction_a &= ~(0b111 << 29);
    fraction_b &= ~(0b111 << 29);

    fraction_a |= 1 << 29;
    fraction_b |= 1 << 29;

    if (exp_diff > 0) {
        fraction_b >>= exp_diff;
    } else {
        fraction_a >>= -exp_diff;
    }

    if (negative1) { fraction_a = -fraction_a; }
    if (negative2) { fraction_b = -fraction_b; }

    unsigned int result_fraction = fraction_a + fraction_b;

    if (result_fraction == 0) {
        result = 0;
        return result;
    }

    int result_sign = result_fraction & 0x80000000;
    if (result_sign) {
        result_fraction = -result_fraction;
    }

    int result_exp;
    if (exp_diff < 0) {
        result_exp = exp_b;
    } else {
        result_exp = exp_a;
    }

    uint32_t counter = 0;
    uint32_t mask = 0x40000000;
    while (!(result_fraction & mask)) {
        counter++;
        result_fraction <<= 1;
    }
    result_fraction <<= 2;
    result_exp += 1 - counter;

    result = encode_posit((repr) {.is_negative=result_sign, .exp=result_exp, .fraction=result_fraction});

    return result;
}

double p32_to_double(posit a) {
    if (a == 0) {
        return 0;
    }

    int sign = 0;
    if (a >> 31) {
        a = ~a + 1;
        sign = 1;
    }

    int first_regime_bit = (a >> 30) & 1;
    int shifts = 0;
    int current_bit = first_regime_bit;
    while (current_bit == first_regime_bit) {
        shifts++;
        current_bit = (a >> (30 - shifts)) & 1;
    }
    int exp = (a >> (30 - shifts - 3)) & 0b111;
    if (first_regime_bit == 1) {
        exp |= (shifts - 1) << 3;
    } else {
        exp |= (-(shifts << 3)) | exp;
    }

    uint32_t fraction = a << (5 + shifts); // 5 = 3 exp + 1 stop + 1 sig
    union {
        float input;
        uint64_t bits;
    } val;
    val.bits = 0;
    if (sign) {
        val.bits |= 0x80000000; // sign
    }
    val.bits |= ((exp + 127) << 23); // exp
    val.bits |= fraction >> 9;
    return val.input;
}

posit p32_mul(posit a, posit b) {
    int neg_flag = 0;

    if (a >> 31 == 1) {
        neg_flag += 1;
        a = -a;
    }
    if (b >> 31 == 1) {
        neg_flag += 1;
        b = -b;
    }

    int first_regime_bit = (a >> 30) & 1;
    int shifts = 0;

    int current_bit = first_regime_bit;
    while (current_bit == first_regime_bit) {
        current_bit = (a >> (29 - shifts)) & 1;
        shifts++;
    }
    int exp_a = (a >> (30 - shifts - 3)) & 0b111;

    int regime_a;
    if (first_regime_bit == 1) {
        regime_a = shifts - 1;
    } else {
        regime_a = -1 * shifts;
    }

    int frac_len_a = 32 - (shifts + 1) - 3 - 1;
    long long fraction_a = a & ((1 << frac_len_a) - 1);

    first_regime_bit = (b >> 30) & 1;
    shifts = 0;
    current_bit = first_regime_bit;
    while (current_bit == first_regime_bit) {
        current_bit = (b >> (29 - shifts)) & 1;
        shifts++;
    }
    int exp_b = (b >> (30 - shifts - 3)) & 0b111;

    int regime_b;
    if (first_regime_bit == 1) {
        regime_b = shifts - 1;
    } else {
        regime_b = -1 * shifts;
    }

    int frac_len_b = 32 - (shifts + 1) - 3 - 1;
    long long fraction_b = b & ((1 << frac_len_b) - 1);

    int new_sign = (a >> 31) ^(b >> 31);
    int new_regim = regime_a + regime_b;
    int new_exp = exp_a + exp_b;

    if (new_exp > 7) {
        new_regim += 1;
        new_exp = new_exp % 8;
    }

    int new_regime_length;
    int bin_new_regime;
    if (new_regim >= 0) {
        new_regime_length = 2 + new_regim;
        bin_new_regime = (1 << new_regime_length) - 2;
    } else {
        new_regime_length = new_regim + 1;
        bin_new_regime = 1;
    }

    long long new_fraction = ((fraction_a * fraction_b) +
                              (fraction_a << frac_len_b) +
                              (fraction_b << frac_len_a));
    int new_fraction_length = 32 - 1 - new_regime_length - 3;
    int l = frac_len_a + frac_len_b;

    if (new_fraction > (((long long) 1 << l) - 1)) {
        l += 1;
        new_exp += 1;

        int c = new_fraction >> (l - 1);
        long long mask = (((long long) 1 << (l - 1)) - 1);

        if (c == 1) {
            //printf("%d %lli", l, (((long long)1 << (l - 1)) - 1));
            new_fraction &= mask;
        } else {
            new_fraction &= mask;
            new_fraction |= (long long) 1 << (l - 1);
        }

    }

    if (l > new_fraction_length) {
        new_fraction = new_fraction >> (l - new_fraction_length);
    } else if (l < new_fraction_length) {
        new_fraction = new_fraction << (new_fraction_length - l);
    }

    posit result;
    result = 0;

    result |= new_sign << 31;
    result |= bin_new_regime << (32 - 1 - new_regime_length);
    result |= new_exp << (32 - 1 - new_regime_length - 3);
    result |= new_fraction;

    if (neg_flag % 2 == 1) {
        result = -result;
    }

    return result;
}