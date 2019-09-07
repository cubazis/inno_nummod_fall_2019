//
// Created by cubazis on 18.08.2019.
//

#include "posit.h"
#include <stdio.h>
#include <stdint.h>

#define BUF_SIZE 33

int test_function(void)
{
	return 42;
}

// fraction should be aligned to the left
posit encodePosit(int isNegative, int exp, unsigned int fraction) {
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';


    posit result;
    result.data = 0;
    int exp_initial = exp;
    exp = exp & 0b111;
    int regime;
    int regime_shifts;

    if (exp_initial >= 0) {
        regime = (((uint32_t)exp_initial) >> 3) + 1;

        regime_shifts = regime;

        for(int i = 0; i < regime; i++) {
            result.data |= 1 << (30-i);
        }
    } else {
        regime = ((uint32_t)-(exp_initial - exp)) >> 3;
        regime_shifts = regime;
        result.data |= 1 << (30-regime);
    }


    result.data |= exp << (30-regime_shifts-3);

    result.data |= fraction >> (1+regime_shifts+4);

    if ((fraction >> (4+regime_shifts))&1) {
        result.data += 1; // rounding
    }

    if (isNegative) {
        result.data = ~result.data+1;
    }

    return result;
}

posit int2posit(int input) {
    posit result;
    result.data = 0;
    if (input == 0) {
        return result;
    };

    unsigned int a;
    if (input > 0){
        a = (unsigned int)input;
    } else {
        a = (unsigned int)-input;
    }

    int shifts = 0;
    unsigned int mask = 0x80000000; // 1 << 31
    int exp;

    //  Align the leading 1 of the significand to the hidden-1
    //  position.  Count the number of shifts required.
    while ((a & mask) == 0)
    {
        a <<= 1;
        shifts++;
    }
    exp = 32 - shifts - 1; //exp of integer
    a <<= 1;

    return encodePosit(input < 0, exp, a);
}

posit double2posit(double input) {

    if (input == 0.0) {
        posit result;
        result.data = 0;
        return result;
    }

    union {
        double input;
        uint64_t bits;
    } val;
    val.input = input;
    int exp = (int)((val.bits >> 52)&0x7FF) - 1023; // substract zero offset
    uint32_t fraction = (uint32_t)((val.bits >> 20)&0xFFFFFFFF); // fit fraction into 32 bit container
    return encodePosit(input < 0, exp, fraction);
}

posit sum(posit a, posit b) {

    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';

    posit result;

    if (a.data == 0) return b;
    if (b.data == 0) return a;

    uint32_t negative1 = (a.data & 0x80000000) >> 31;
    if (negative1) a.data = -a.data; // convert 2s complement back


    uint32_t negative2 = (b.data & 0x80000000) >> 31;
    if (negative2) b.data = -b.data; // convert 2s complement back

    int exp_a = 0;
    int exp_b = 0;

    // decode first num
    int first_regime_bit = (a.data >> 30) & 1;
    int shifts = 0;
    int shifts_a = 0;
    int shifts_b = 0;
    int current_bit = first_regime_bit;
    while(current_bit == first_regime_bit) {
        current_bit = (a.data >> (29-shifts))&1;
        shifts++;
    }
    int exp = (a.data >> (30-shifts-3))&0b111;
    if (first_regime_bit == 1) {
        exp |= (shifts-1) << 3;
    } else {
        exp |= (-(shifts << 3)) | exp;
    }
    if (a.data & 0x80000000) exp = -exp;
    exp_a = exp;
    shifts_a = shifts;

    // decode 2nd num
    first_regime_bit = (b.data >> 30) & 1;
    shifts = 0;
    current_bit = first_regime_bit;
    while(current_bit == first_regime_bit) {
        current_bit = (b.data >> (29-shifts))&1;
        shifts++;
    }
    exp = (b.data >> (30-shifts-3))&0b111;

    if (first_regime_bit == 1) {
        exp |= (shifts-1) << 3;
    } else {
        exp |= (-(shifts << 3)) | exp;
    }
    if (b.data & 0x80000000) exp = -exp;
    exp_b = exp;
    shifts_b = shifts;

    // next step
    int exp_diff = exp_a - exp_b;

    int fraction_a = a.data;
    fraction_a <<= 2+shifts_a; // usually 5 + shifts_a but we need 3 bites before so 5-3=2

    int fraction_b = b.data;
    fraction_b <<= 2+shifts_b;

    fraction_a &= ~(0b111 << 29);
    fraction_b &= ~(0b111 << 29);

    fraction_a |= 1 << 29; // add first 1 that was removed after normalization
    fraction_b |= 1 << 29; // add first 1 that was removed after normalization

    if (exp_diff > 0) {
        fraction_b >>= exp_diff;
    } else {
        fraction_a >>= -exp_diff;
    }



    if (negative1) {
        fraction_a = -fraction_a;
    }
    if (negative2) {
        fraction_b = -fraction_b; // set 1 if negative
    }
    // if (negative1) fraction_a = -fraction_a;


    unsigned int result_fraction = fraction_a+fraction_b;

    if (result_fraction == 0) {
        result.data = 0;
        return result;
    }

    int result_sign = result_fraction & 0x80000000;
    if (result_sign) {
        result_fraction = -result_fraction;
    }


    int result_exp; // find biggest
    if (exp_diff < 0) {
        result_exp = exp_b;
    } else {
        result_exp = exp_a;
    }

    uint32_t counter = 0;
    uint32_t mask = 0x40000000;
    while(!(result_fraction & mask)) {
        counter++;
        result_fraction <<= 1;
    }
    result_fraction <<= 2;
    result_exp += 1 - counter;

    result = encodePosit(result_sign, result_exp, result_fraction);

    //printf("%d\n", fraction);
    return result;
}

float posit2float(posit a) {

    if (a.data == 0) {
        return 0.0;
    }

    int sign = 0;
    if (a.data >> 31) {
        a.data = ~(a.data) + 1;
        sign = 1;
    }

    int first_regime_bit = (a.data >> 30) & 1;
    int shifts = 0;
    int current_bit = first_regime_bit;
    while(current_bit == first_regime_bit) {
        shifts++;
        current_bit = (a.data >> (30-shifts))&1;
    }
    int exp = (a.data >> (30-shifts-3))&0b111;
    if (first_regime_bit == 1) {
        exp |= (shifts-1) << 3;
    } else {
        exp |= (-(shifts << 3)) | exp;
    }


    uint32_t fraction = a.data << (5+shifts); // 5 = 3 exp + 1 stop + 1 sig
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

posit negate(posit a) {
    posit r = a;
    r.data = -r.data;
    return r;
}

posit diff(posit a, posit b) {
    return sum(a, negate(b));
}

int lessthan(posit a, posit b) {
    posit d = diff(a, b);
    return (d.data & 0x80000000) >> 31;
}

int biggerthan(posit a, posit b) {
    return lessthan(b, a);
}

posit divide(posit a, int b) {
    if (b == 2) {
        int sign = 0;
        if (a.data >> 31) {
            a.data = ~(a.data) + 1;
            sign = 1;
        }

        int first_regime_bit = (a.data >> 30) & 1;
        int shifts = 0;
        int current_bit = first_regime_bit;
        while(current_bit == first_regime_bit) {
            shifts++;
            current_bit = (a.data >> (30-shifts))&1;
        }
        int exp = (a.data >> (30-shifts-3))&0b111;
        if (first_regime_bit == 1) {
            exp |= (shifts-1) << 3;
        } else {
            exp |= (-(shifts << 3)) | exp;
        }
        uint32_t fraction = a.data << (5+shifts); // 5 = 3 exp + 1 stop + 1 sig

        return encodePosit(sign, exp-1, fraction);
    }

    posit p;
    p.data = 0;
    return p;
}

posit multiplication(posit a, posit b) {
    //check negative values
    int neg_flag = 0;

    if (a.data >> 31 == 1) {
        neg_flag += 1;
        a.data = -a.data;
    }
    if (b.data >> 31 == 1) {
        neg_flag += 1;
        b.data = -b.data;
    }

    // decode first num
    int first_regime_bit = (a.data >> 30) & 1;
    int shifts = 0;

    int current_bit = first_regime_bit;
    while(current_bit == first_regime_bit) {
        current_bit = (a.data >> (29-shifts))&1;
        shifts++;
    }
    int exp_a = (a.data >> (30-shifts-3))&0b111;

    int regim_a;
    if (first_regime_bit == 1) {
        regim_a = shifts - 1;
    }
    else {
        regim_a = -1 * shifts;
    }

    int frac_len_a = 32 - (shifts + 1) - 3 - 1;
    long long fraction_a = a.data & ((1 << frac_len_a) - 1);

    // decode 2nd num
    first_regime_bit = (b.data >> 30) & 1;
    shifts = 0;
    current_bit = first_regime_bit;
    while(current_bit == first_regime_bit) {
        current_bit = (b.data >> (29-shifts))&1;
        shifts++;
    }
    int exp_b = (b.data >> (30-shifts-3))&0b111;

    int regim_b;
    if (first_regime_bit == 1) {
        regim_b = shifts - 1;
    }
    else {
        regim_b = -1 * shifts;
    }

    int frac_len_b = 32 - (shifts + 1) - 3 - 1;
    long long fraction_b = b.data & ((1 << frac_len_b) - 1);

    int new_sign = (a.data >> 31) ^ (b.data >> 31);
    int new_regim = regim_a + regim_b;
    int new_exp = exp_a + exp_b;

    if (new_exp > 7) {
        new_regim += 1;
        new_exp = new_exp % 8;
    }

    int new_regim_len;
    int bin_new_reg;
    if (new_regim >= 0) {
        new_regim_len = 2 + new_regim;
        bin_new_reg = (1 << new_regim_len) - 2;
    }
    else
    {
        new_regim_len = new_regim + 1;
        bin_new_reg = 1;
    }

    long long new_fraction = ((fraction_a * fraction_b) +
                              (fraction_a << frac_len_b) +
                              (fraction_b << frac_len_a));
    int new_fraction_length = 32 - 1 - new_regim_len - 3;
    int l = frac_len_a + frac_len_b;

    if (new_fraction > (((long long)1 << l) - 1)) {
        l += 1;
        new_exp += 1;

        int c = new_fraction >> (l - 1);
        long long mask = (((long long)1 << (l - 1)) - 1);

        if (c == 1) {
            //printf("%d %lli", l, (((long long)1 << (l - 1)) - 1));
            new_fraction &= mask;
        }
        else
        {
            new_fraction &= mask;
            new_fraction |= (long long)1 << (l - 1);
        }

    }

    if (l > new_fraction_length) {
        new_fraction = new_fraction >> (l - new_fraction_length);
    }
    if (l < new_fraction_length) {
        new_fraction = new_fraction << (new_fraction_length - l);
    }

    posit result;
    result.data = 0;

    result.data |= new_sign << 31;
    result.data |= bin_new_reg << (32 - 1 - new_regim_len);
    result.data |= new_exp << (32 - 1 - new_regim_len - 3);
    result.data |= new_fraction;

    if (neg_flag % 2 == 1) {
        result.data = -result.data;
    }

    return result;
}

posit posit_pow(posit a, int b) {
    posit result = double2posit(1.0);
    for (int i = 1; i <= b; i++) {
        result = multiplication(result, a);
    }
    return result;
}

posit posit_divide(posit a, int b) {
    if (b == 2) {
        int sign = 0;
        if (a.data >> 31) {
            a.data = ~(a.data) + 1;
            sign = 1;
        }

        int first_regime_bit = (a.data >> 30) & 1;
        int shifts = 0;
        int current_bit = first_regime_bit;
        while(current_bit == first_regime_bit) {
            shifts++;
            current_bit = (a.data >> (30-shifts))&1;
        }
        int exp = (a.data >> (30-shifts-3))&0b111;
        if (first_regime_bit == 1) {
            exp |= (shifts-1) << 3;
        } else {
            exp |= (-(shifts << 3)) | exp;
        }
        uint32_t fraction = a.data << (5+shifts); // 5 = 3 exp + 1 stop + 1 sig

        return encodePosit(sign, exp-1, fraction);
    }
    return double2posit(0.0);
}

int isNegative(posit a) {
    return a.data & 0x80000000;
}

posit posit_abs(posit a) {
    if (isNegative(a)) return negate(a);
    return a;
}

int beq(posit a, posit b) {
    return biggerthan(a, b) || (a.data == b.data);
}

