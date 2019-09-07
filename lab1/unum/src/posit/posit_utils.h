#ifndef POSIT_32_POSIT_UTILS_H
#define POSIT_32_POSIT_UTILS_H

#include <stdbool.h>
#include "stdint.h"
#include "posit.h"

int calc_scale(int reg, uint32_t exp, int es);
posit_unpack_t parse_scale(int scale, int es);
int calc_posit_scale(posit_unpack_t pu);

bool is_zero(posit_unpack_t v);
bool is_nar(posit_unpack_t v);
posit_unpack_t get_posit_nar();
posit_unpack_t get_posit_zero();

uint8_t regime_to_size(posit_unpack_t v);
int size_to_regime(uint8_t fb, uint8_t s);

uint8_t sign_size();
uint8_t regime_size(posit32_t v);
uint8_t exponent_size(posit32_t v, uint8_t rs);
uint8_t fraction_size(posit32_t v, uint8_t rs);

uint8_t estimate_fraction_size(posit_unpack_t v);
uint32_t fraction_with_hb(uint32_t f);
uint32_t fraction_remove_hn(uint32_t f);

posit32_t toggle_sign(posit32_t v);

#endif //POSIT_32_POSIT_UTILS_H
