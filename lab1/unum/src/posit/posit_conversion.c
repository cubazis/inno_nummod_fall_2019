#include "stdint.h"
#include <math.h>
#include "posit.h"
#include "posit_utils.h"
#include "../double/double.h"
#include "../double/double_conversion.h"
#include "../utils.h"

posit_unpack_t unpack_posit(posit32_t p) {
    posit_unpack_t pu;

    pu.sign = RSHIFT(p.value, POSIT32_SIZE - 1);
    if (pu.sign == 1) {
        p = toggle_sign(p);
    }

    uint8_t r_sign = RSHIFT(p.value, POSIT32_SIZE - 2);

    uint8_t s_size = sign_size();
    uint8_t r_size = regime_size(p);
    uint8_t e_size = exponent_size(p, r_size);
    uint8_t f_size = fraction_size(p, r_size);

    int8_t real_r_size = r_size - 1;
    int regime = size_to_regime(r_sign, real_r_size);

    pu.regime = regime;
    pu.exponent = CUT_BITS(p.value, s_size + r_size, e_size);
    pu.fraction = LSHIFT(p.value, s_size + r_size + e_size);

    return pu;
}

posit32_t pack_posit(posit_unpack_t pu) {
    posit32_t p;
    p.value = 0;

    uint8_t s_size = sign_size();
    uint8_t r_size = regime_to_size(pu);
    uint8_t full_r_size = r_size + 1;
    uint8_t e_size = MIN(POSIT32_ES_SIZE, POSIT32_SIZE - s_size - full_r_size);
    uint8_t f_size = MAX(POSIT32_SIZE - s_size - full_r_size - e_size, 0);

    uint32_t r_bits = pu.regime >= 0 ? LSHIFT(RSHIFT(FILLED_MASK, POSIT32_SIZE - r_size), 1) : LSB_MASK;
    uint32_t e_bits = CUT_BITS(pu.exponent, POSIT32_SIZE - e_size, e_size);
    uint32_t f_bits = RSHIFT(pu.fraction, POSIT32_SIZE - f_size);

    e_bits = LSHIFT(e_bits, f_size);
    r_bits = LSHIFT(r_bits, f_size + e_size);

    p.value = f_bits | e_bits | r_bits;

    if (pu.sign == 1) {
        p = toggle_sign(p);
    }

    return p;
}

posit_unpack_t from_double(double val) {
    posit_unpack_t pu;

    double_unpack_t du = unpack_double(val);

    int scale = du.exponent - DOUBLE_EX_OFFSET;
    int fraction_offset = DOUBLE_MA_SIZE - POSIT32_SIZE;

    // Calculate exponent and regime
    posit_unpack_t tpu = parse_scale(scale, POSIT32_ES_SIZE);

    pu.sign = du.sign;
    pu.regime = tpu.regime;
    pu.exponent = tpu.exponent;
    pu.fraction = RSHIFT(du.mantissa, fraction_offset);

    return pu;
}

double to_double(posit_unpack_t val) {
    if(is_nar(val)) {
        return INFINITY;
    }
    if(is_zero(val)) {
        return 0;
    }

    double_unpack_t du;

    int scale = calc_posit_scale(val);

    int est_fraction_size = estimate_fraction_size(val);
    int fraction_offset = DOUBLE_MA_SIZE - POSIT32_U_SIZE;

    // TODO handle overflow and underflow

    du.sign = val.sign;
    du.exponent = scale + DOUBLE_EX_OFFSET;
    du.mantissa = LSHIFT((uint64_t) val.fraction, fraction_offset);

    return pack_double(du);
}
