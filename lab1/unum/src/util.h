#ifndef __POSIT_UTIL_H
#define __POSIT_UTIL_H

#include "posit_types.h"

#define CLZ(n) \
    ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define LSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define RSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define POW2(n) \
    (LSHIFT(1, (n)))

#define FLOORDIV(a, b) \
    ((a) / (b) - ((a) % (b) < 0))

#define MIN(a, b) \
    ((a) < (b) ? (a) : (b))

#define MAX(a, b) \
    ((a) > (b) ? (a) : (b))

#define LMASK(bits, size) \
    ((bits) & LSHIFT(POSIT_MASK, NBITS - (size)))

#define HIDDEN_BIT(frac) \
    (POSIT_MSB | RSHIFT((frac), 1))

bool util_is_zero(POSIT_UTYPE p);
bool util_is_nar(POSIT_UTYPE p);
bool util_is_neg(POSIT_UTYPE p);

int util_ss(void);
int util_rs(POSIT_UTYPE p);
int util_es(POSIT_UTYPE p);
int util_fs(POSIT_UTYPE p);

POSIT_UTYPE util_neg(POSIT_UTYPE p);

#endif

