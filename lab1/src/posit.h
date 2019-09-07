#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#include <stdint.h>
#include <stdbool.h>

#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define LSHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define RSHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define POW2(n) (LSHIFT(1, (n)))

#define HIDDEN_BIT(a) ((a >> 1) | MSB)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MSB ((uint32_t) 0x80000000)
#define MASK ((uint32_t)0xFFFFFFFF)
#define ES 4
#define POSIT_SIZE 32
#define MAX_EXP (30 * 16)

typedef struct _posit32 {
    unsigned int bits;
} posit;

posit double_to_posit(double a);

posit add(posit a, posit b);

posit sub(posit a, posit b);

posit mul(posit a, posit b);

posit power(posit a, posit b);

posit negate(posit a);

double posit_to_double(posit a);

#endif //UNUM_POSIT_H