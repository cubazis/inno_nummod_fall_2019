#ifndef POSIT_32_POSIT_CONVERSION_H
#define POSIT_32_POSIT_CONVERSION_H

#include "posit.h"
#include "../double/double.h"

posit_unpack_t unpack_posit(posit32_t p);
posit32_t pack_posit(posit_unpack_t pu);

posit_unpack_t from_double(double val);
double to_double(posit_unpack_t val);

#endif //POSIT_32_POSIT_CONVERSION_H
