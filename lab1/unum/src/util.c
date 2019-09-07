#include "util.h"
#include <stdint.h>
#include <stdbool.h>

#define POSIT_WIDTH     32
#define POSIT_ZERO      ((uint32_t)0x00000000)
#define POSIT_ONE       ((uint32_t)0x40000000)
#define POSIT_NAR       ((uint32_t)0x80000000)
#define POSIT_MSB       ((uint32_t)0x80000000)
#define POSIT_MASK      ((uint32_t)0xFFFFFFFF)

bool util_is_zero(uint32_t p)
{
    return p == POSIT_ZERO;
}

bool util_is_nar(uint32_t p)
{
    return p == POSIT_NAR;
}

bool util_is_neg(uint32_t p)
{
    return (int32_t)p < 0 && !util_is_nar(p);
}

int util_ss()
{
    return 1;
}

int util_rs(uint32_t p, int nbits)
{
    int ss = util_ss();
    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss));
    int rs = MAX(lz, lo) + 1;

    return MIN(rs, nbits - ss);
}

int util_es(uint32_t p, int nbits, int es)
{
    int ss = util_ss();
    int rs = util_rs(p, nbits);

    return MIN(MAX(nbits - ss - rs, 0), es);
}

int util_fs(uint32_t p, int nbits, int es)
{
    int ss = util_ss();
    int rs = util_rs(p, nbits);

    return MAX(nbits - ss - rs - es, 0);
}

uint32_t util_neg(uint32_t p, int nbits)
{
    return LMASK(-LMASK(p, nbits), nbits);
}
