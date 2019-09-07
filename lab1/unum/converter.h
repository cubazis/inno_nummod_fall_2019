//
// Created by arsee on 9/5/2019.
//

#ifndef UNUM32_CONVERTER_H
#define UNUM32_CONVERTER_H


#include <stdint.h>

uint32_t double_to_posit(double d);
double fromPosit(uint32_t p);
struct unpacked_posit{
    int negative;
    int32_t exp;
    uint32_t frac;
};


struct unpacked_posit unpack_posit(uint32_t p);
uint32_t pack_posit(int neg, int32_t exp, uint32_t frac);
#endif //UNUM32_CONVERTER_H

