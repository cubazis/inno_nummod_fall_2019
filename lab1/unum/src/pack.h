#ifndef __POSIT_PACK_H
#define __POSIT_PACK_H

#include "posit_types.h"

#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

typedef struct unpacked_t
{
    bool neg;
    POSIT_STYPE exp;
    POSIT_UTYPE frac;
} unpacked;

POSIT_UTYPE pack_posit(unpacked up);
float pack_float(unpacked up);
double pack_double(unpacked up);

unpacked unpack_posit(POSIT_UTYPE p);
unpacked unpack_float(float f);
unpacked unpack_double(double f);

#endif
