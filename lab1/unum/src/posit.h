
#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#include <stdint.h>
#include <stdbool.h>

#define ES 2
#define NBITS 32

#define ZERO      ((uint32_t)0x00000000)
#define ONE       ((uint32_t)0x40000000)
#define MSB       ((uint32_t)0x80000000)
#define MASK      ((uint32_t)0xFFFFFFFF)

#define LMASK(bits, size) ((bits) & (MASK<<(NBITS - (size))))
#define HIDDEN_BIT(frac) (MSB | (frac>> 1))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define FLOORDIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

typedef struct _posit32 {
    bool neg;
    int32_t exp;
    uint32_t frac;
    uint32_t v;
} posit;

posit get_posit_from_double(double f);

double get_double(posit);

uint32_t get_posit_from_bits(bool neg, int32_t exp, uint32_t frac);

posit add(posit lhs, posit rhs);

posit subtract(posit lhs, posit rhs);

posit multiply(posit lhs, posit rhs);

posit power(posit lhs, posit rhs);

posit posit_abs(posit p);

bool is_less(posit lhs, posit rhs);

posit add_(posit lhs, posit rhs, bool);

posit sub_(posit lhs, posit rhs, bool);

bool same_opposite(posit, posit, bool);

bool is_zero(posit p);

int get_sign_size(void);

posit get_neg_p(posit);

uint32_t get_neg(int32_t);

int test_function(void);

#endif //UNUM_POSIT_H