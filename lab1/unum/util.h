#ifndef __POSIT_UTIL_H
#define __POSIT_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __GNUC__

#define CLZ(n) \
    ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))
#endif

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
    ((bits) & LSHIFT(POSIT_MASK, POSIT_WIDTH - (size)))

#define HIDDEN_BIT(frac) \
    (POSIT_MSB | RSHIFT((frac), 1))

int util_is_zero(uint32_t p);
int util_is_nar(uint32_t p);
int util_is_neg(uint32_t p);


int util_rs(uint32_t p, int nbits);

uint32_t util_neg(uint32_t p, int nbits);

#ifdef __cplusplus
}
#endif

#endif
