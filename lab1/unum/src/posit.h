//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include "posit_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define nBits 32
#define Es 4

#define q_size 3

#ifdef __GNUC__
#if nBits > 32

#error "CLZ doesn't support 64-bit"
#endif
#define CLZ(n) \
    ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))
#endif


// shift count wraps around on x86: https://stackoverflow.com/q/3871650
#define LSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

// shift count wraps around on x86: https://stackoverflow.com/q/3871650
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
    ((bits) & LSHIFT(((uint32_t)0xFFFFFFFF), nBits - (size)))

#define HIDDEN_BIT(frac) \
    (((uint32_t)0x80000000) | RSHIFT((frac), 1))




typedef struct _posit32 {

    uint32_t reprBits;

} posit32;


typedef struct simple_quire{

    posit32 pos[q_size];

} simple_quire;


struct float_struct
{
    bool neg;
    int32_t exp;
    uint32_t frac;
};

bool isZero(posit32 a);


bool isNar(posit32 a);


struct float_struct divide_float(float f);
struct float_struct divide_double(double f);
posit32 construct_from_fl(struct float_struct up);
float getFloatRepr_new(posit32 p);

//constructor
posit32 posit32_init(float val);
posit32 posit32_init_fromparts(uint32_t reprBits);
struct float_struct unpack_posit_new(uint32_t repr);
struct float_struct op2_mul(struct float_struct p1, struct float_struct p2);
int util_conv_func(uint32_t p, int nbits);
float pack_float_new(struct float_struct up);
double pack_double_new(struct float_struct up);
double getDouble(posit32 a);


struct float_struct sub(struct float_struct a, struct float_struct b, bool neg);
struct float_struct add(struct float_struct a, struct float_struct b, bool neg);

struct float_struct op2_mul_new(struct float_struct a, struct float_struct b);
struct float_struct op2_div_new(struct float_struct a, struct float_struct b);
struct float_struct op2_add_new(struct float_struct a, struct float_struct b);
struct float_struct op2_sub_new(struct float_struct a, struct float_struct b);
posit32 add32(posit32 p1, posit32 p2);
posit32 sub32(posit32 p1, posit32 p2);

simple_quire pow_large(posit32 a, long double b);
simple_quire negate(simple_quire a);
posit32 get_pos_toconvert(long double * num, int quire_i);
posit32 posit_init_double(double a);


long double q_t_d(simple_quire a);


#ifdef __cplusplus
}
#endif

int test_function(void);

#endif //UNUM_POSIT_H
