#ifndef POSIT_32_UTILS_H
#define POSIT_32_UTILS_H

#include "stdint.h"

#define CLZ(n)  ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))
#define CTZ(n)  ((n) == 0 ? 8 * sizeof(n) : __builtin_ctz(n))

#define RSHIFT(bits, offset)  ((offset) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (offset))
#define LSHIFT(bits, offset)  ((offset) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (offset))
#define CUT_BITS(bits, start, count)  RSHIFT(LSHIFT(bits, start), sizeof(bits) * 8 - count)

#define POW2(n)  (LSHIFT(1, (n)))
#define ABS(n)   (n < 0 ? -n : n)

#define INVERT_BOOL(b)  (b == 0 ? 1 : 0)

#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

void print_num(uint32_t num);
void print_bits(long num);
void print_double_bits(double num);

#endif //POSIT_32_UTILS_H
