#ifndef __POSIT_UTIL_H
#define __POSIT_UTIL_H
#include <stdint.h>
#include <stdbool.h>

#define POSIT_WIDTH     32
#define POSIT_ZERO      ((uint32_t)0x00000000)
#define POSIT_ONE       ((uint32_t)0x40000000)
#define POSIT_NAR       ((uint32_t)0x80000000)
#define POSIT_MSB       ((uint32_t)0x80000000)
#define POSIT_MASK      ((uint32_t)0xFFFFFFFF)

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __GNUC__
#if POSIT_WIDTH > 32
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
    ((bits) & LSHIFT(POSIT_MASK, POSIT_WIDTH - (size)))

#define HIDDEN_BIT(frac) \
    (POSIT_MSB | RSHIFT((frac), 1))

bool util_is_zero(uint32_t p);
bool util_is_nar(uint32_t p);
bool util_is_neg(uint32_t p);

int util_ss();
int util_rs(uint32_t p, int nbits);
int util_es(uint32_t p, int nbits, int es);
int util_fs(uint32_t p, int nbits, int es);

uint32_t util_neg(uint32_t p, int nbits);

#ifdef __cplusplus
}
#endif

#endif
