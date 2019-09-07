#include <stdint.h>
#include "util.h"
#include "posit_constants.h"



int util_is_zero(uint32_t p)
{
    return p == POSIT_ZERO;
}

int util_is_nar(uint32_t p)
{
    return p == POSIT_NAR;
}

int util_is_neg(uint32_t p)
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



uint32_t util_neg(uint32_t p, int nbits)
{
    // reverse all bits and summ one
    return LMASK(-LMASK(p, nbits), nbits);
}
