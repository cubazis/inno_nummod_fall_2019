#ifndef POSIT_32_POSIT_OPS_H
#define POSIT_32_POSIT_OPS_H

#include "posit.h"

int posit_is_less(posit_unpack_t v1, posit_unpack_t v2);

posit_unpack_t posit_neg(posit_unpack_t v);
posit_unpack_t posit_abs(posit_unpack_t v);

posit_unpack_t posit_add(posit_unpack_t v1, posit_unpack_t v2);
posit_unpack_t posit_sub(posit_unpack_t v1, posit_unpack_t v2);

posit_unpack_t posit_mul(posit_unpack_t v1, posit_unpack_t v2);
posit_unpack_t posit_div(posit_unpack_t v1, posit_unpack_t v2);

posit_unpack_t posit_pow(posit_unpack_t v, posit_unpack_t p);
posit_unpack_t posit_dot_product(posit_unpack_t *left, posit_unpack_t *right, int size);

#endif //POSIT_32_POSIT_OPS_H
