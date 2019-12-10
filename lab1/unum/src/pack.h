#ifndef __POSIT_PACK_H
#define __POSIT_PACK_H

#include "posit_types.h"

typedef struct unpacked_t
{
    bool neg;
    POSIT_STYPE exp;
    POSIT_UTYPE frac;
} unfold;

POSIT_UTYPE pack_posit(unfold up);
float pack_float(unfold up);
double pack_double(unfold up);

unfold unpack_posit(POSIT_UTYPE p);

#endif

