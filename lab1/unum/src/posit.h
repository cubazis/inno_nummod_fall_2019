//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define _uint64 unsigned long long
#define bool short
#define _uint32 unsigned int

#define LSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define RSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define POW2(n) \
    (LSHIFT(1, (n)))

#define FLOORDIV(a, b) \
    ((a) / (b) - ((a) % (b) < 0))

#define HIDDEN_BIT(a) \
    ((a >> 1) | MSB)

#define MIN(a, b) \
    ((a) < (b) ? (a) : (b))

#define MAX(a, b) \
    ((a) > (b) ? (a) : (b))

typedef struct _posit32 {
    unsigned int bits;
} posit;

posit fromDouble(double a);

double toDouble(posit a);

posit add(posit a, posit b);

posit sub(posit a, posit b);

posit multiply(posit a, posit b);

//posit divide(posit a, posit b);

posit power(posit a, posit b);

posit negate(posit a);

int test_function(void);

#endif //UNUM_POSIT_H
