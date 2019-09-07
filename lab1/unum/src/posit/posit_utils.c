#include <stdbool.h>
#include "stdint.h"
#include "posit.h"
#include "../utils.h"

int calc_scale(int reg, uint32_t exp, int es) {
    uint32_t base = POW2(es);

    int scale = base * reg + exp;

    return scale;
}

posit_unpack_t parse_scale(int scale, int es) {
    int base = POW2(es);

    int reg = scale / base - (scale % base < 0);
    int exp = scale - base * reg;

    posit_unpack_t pu = {0, reg, exp, 0};

    return pu;
}

int calc_posit_scale(posit_unpack_t pu) {
    return calc_scale(pu.regime, pu.exponent, POSIT32_ES_SIZE);
}

bool is_zero(posit_unpack_t v) {
    return v.sign == 0 && v.regime == 0 && v.exponent == 0 && v.fraction == 0;
}

bool is_nar(posit_unpack_t v) {
    return v.sign == 1 && v.regime == 0 && v.exponent == 0 && v.fraction == 0;
}

posit_unpack_t get_posit_nar() {
    posit_unpack_t pu = {1, 0, 0, 0};

    return pu;
}

posit_unpack_t get_posit_zero() {
    posit_unpack_t pu = {0, 0, 0, 0};

    return pu;
}

uint8_t regime_to_size(posit_unpack_t v) {
    return v.regime < 0 ? -v.regime : v.regime + 1;
}

int size_to_regime(uint8_t fb, uint8_t s) {
    return fb == 0 ? -s : s - 1;
}

uint8_t sign_size() {
    return 1;
}

uint8_t regime_size(posit32_t v) {
    int ss = sign_size();

    int p_zeros = CLZ(LSHIFT(v.value, ss));
    int p_ones = CLZ(LSHIFT(~v.value, ss));

    int p = MAX(p_zeros, p_ones) + 1;

    return MIN(p, POSIT32_SIZE - ss);
}

uint8_t exponent_size(posit32_t v, uint8_t rs) {
    int ss = sign_size();
    if (rs == 0) {
        rs = regime_size(v);
    }

    return MIN(POSIT32_ES_SIZE, MAX(POSIT32_SIZE - ss - rs, 0));
}

uint8_t fraction_size(posit32_t v, uint8_t rs) {
    int ss = sign_size();
    if (rs == 0) {
        rs = regime_size(v);
    }

    return MAX(POSIT32_SIZE - ss - rs - POSIT32_ES_SIZE, 0);
}

uint8_t estimate_fraction_size(posit_unpack_t v) {
    int ss = sign_size();
    int rs = regime_to_size(v) + 1;
    int es = POSIT32_ES_SIZE;

    return MAX(POSIT32_SIZE - ss - rs - es, 0);
}

uint32_t fraction_with_hb(uint32_t f) {
    return (MSB_MASK | RSHIFT(f, 1));
}

uint32_t fraction_remove_hn(uint32_t f) {
    return LSHIFT(f, 1);
}

posit32_t toggle_sign(posit32_t v) {
    v.value = ~v.value + 1;

    return v;
}
