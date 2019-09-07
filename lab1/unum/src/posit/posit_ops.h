#ifndef POSIT_32_POSIT_OPS_H
#define POSIT_32_POSIT_OPS_H

#include "posit.h"

posit_unpack_t posit_add(posit_unpack_t v1, posit_unpack_t v2);
posit_unpack_t posit_sub(posit_unpack_t v1, posit_unpack_t v2);

posit_unpack_t posit_mul(posit_unpack_t v1, posit_unpack_t v2);
posit_unpack_t posit_div(posit_unpack_t v1, posit_unpack_t v2);

posit_unpack_t posit_neg(posit_unpack_t v);

#endif //POSIT_32_POSIT_OPS_H
