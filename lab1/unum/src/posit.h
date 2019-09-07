//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H


#define _uint64 unsigned long long

#define _uint32 unsigned int

#define bool short


#define FLOOR_DIV(a, b) ((a) / (b) - ((a) % (b) < 0))

#define HIDDEN_BIT(a) ((a >> 1) | MSB)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define RIGHT_SHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define LEFT_SHIFT(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define POW2(n) (LEFT_SHIFT(1, (n)))

#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))


typedef struct _posit32 {
    unsigned int bits;
} posit;

posit fromDouble(double a);

double toDouble(posit a);

posit add(posit a, posit b);

posit subtract(posit a, posit b);

posit multiply(posit a, posit b);

posit power(posit a, posit b);

posit negate(posit a);

int test_function(void);

#endif //UNUM_POSIT_H