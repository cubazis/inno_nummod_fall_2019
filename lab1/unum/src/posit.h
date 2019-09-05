//
// Declarations for posit32_2 implementation.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#include <stdint.h>
#include <stdbool.h>

#define ES 2
#define NBITS 32

#define POSIT_UTYPE     uint32_t
#define POSIT_STYPE     int32_t
#define POSIT_LUTYPE    uint64_t
#define POSIT_ZERO      ((POSIT_UTYPE)0x00000000)
#define POSIT_ONE       ((POSIT_UTYPE)0x40000000)
#define POSIT_NAR       ((POSIT_UTYPE)0x80000000)
#define POSIT_MSB       ((POSIT_UTYPE)0x80000000)
#define POSIT_MASK      ((POSIT_UTYPE)0xFFFFFFFF)


#define POW2(n) (LSHIFT(1, (n)))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define FLOORDIV(a, b) ((a) / (b) - ((a) % (b) < 0))

#define HIDDEN_BIT(frac) (POSIT_MSB | RSHIFT((frac), 1))

#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define LMASK(bits, size) ((bits) & LSHIFT(POSIT_MASK, NBITS - (size)))
#define LSHIFT(bits, n) ((n) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (n))
#define RSHIFT(bits, n) ((n) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (n))


typedef struct _posit32 {
    POSIT_UTYPE v;
} posit;

typedef struct _unpacked_posit32 {
    bool neg;
    POSIT_STYPE exp;
    POSIT_UTYPE frac;
} unpacked_posit;

posit make_posit(double);

posit pack_posit(unpacked_posit);
double pack_double(unpacked_posit);

unpacked_posit unpack_posit(posit p);
unpacked_posit unpack_double(double f);

posit add(posit, posit);
posit sub(posit, posit);
unpacked_posit add_(unpacked_posit, unpacked_posit, bool);
unpacked_posit sub_(unpacked_posit, unpacked_posit, bool);

bool same_opposite(unpacked_posit, unpacked_posit, bool);

posit mult(posit, posit);
posit p_pow(posit, posit);

posit p_abs(posit);
bool is_less(posit, posit);

bool is_zero(posit p);
bool is_nar(posit p);
bool is_neg(posit p);

int get_ss(void);
int get_rs(posit p);
int get_es(posit p);
int get_fs(posit p);

posit get_neg_p(posit);
POSIT_UTYPE get_neg(POSIT_STYPE);

int test_function(void);

#endif //UNUM_POSIT_H
