//
// Created by arsee on 9/6/2019.
//

#include "operations.h"



static uint32_t add_posits( uint32_t first, uint32_t second, int neg)
{
    struct unpacked_posit a= unpack_posit(first);
    struct unpacked_posit b= unpack_posit(second);

    struct unpacked_posit r;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    uint64_t frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (RSHIFT(frac, POSIT_WIDTH) != 0) {
        r.exp++;
        frac = RSHIFT(frac, 1);
    }

    r.negative = neg;
    r.frac = LSHIFT(frac, 1);

    return pack_posit(r.negative,r.exp,r.frac);
}


static uint32_t sub_posits(uint32_t first, uint32_t second , int neg)
{
    struct unpacked_posit a= unpack_posit(first);
    struct unpacked_posit b= unpack_posit(second);
    struct unpacked_posit r;

    uint32_t afrac = HIDDEN_BIT(a.frac);
    uint32_t bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac;

    if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        neg = !neg;
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
        frac = bfrac - afrac;
    }

    r.negative = neg;
    r.exp -= CLZ(frac);
    r.frac = LSHIFT(frac, CLZ(frac) + 1);

    return pack_posit(r.negative,r.exp,r.frac);
}

uint32_t summ(uint32_t first, uint32_t second){
    struct unpacked_posit a= unpack_posit(first);
    struct unpacked_posit b= unpack_posit(second);
    if (a.negative == b.negative) {
        return add_posits(first, second, a.negative);
    } else {
        return sub_posits(first, second, a.negative);
    }
}

uint32_t mult(uint32_t first,uint32_t second){
    struct unpacked_posit r;
    struct unpacked_posit a = unpack_posit(first);
    struct unpacked_posit b = unpack_posit(second);
    uint64_t afrac = HIDDEN_BIT(a.frac);
    int64_t bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac = RSHIFT(afrac * bfrac, POSIT_WIDTH);
    int32_t exp = a.exp + b.exp + 1;

    if ((frac & POSIT_MSB) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.negative = a.negative ^ b.negative;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return pack_posit(r.negative,r.exp,r.frac);
}

uint32_t divis(uint32_t first, uint32_t second)
{
    struct unpacked_posit a= unpack_posit(first);
    struct unpacked_posit b= unpack_posit(second);
    struct unpacked_posit r;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    int32_t exp = a.exp - b.exp;

    if (afrac < bfrac) {
        exp--;
        bfrac = RSHIFT(bfrac, 1);
    }

    r.negative = a.negative ^ b.negative;
    r.exp = exp;
    r.frac = LSHIFT(afrac, POSIT_WIDTH) / bfrac;

    return pack_posit(r.negative,r.exp,r.frac);
}

uint32_t dif(uint32_t first, uint32_t second){
    struct unpacked_posit a= unpack_posit(first);
    struct unpacked_posit b= unpack_posit(second);
    if (a.negative == b.negative) {
        return sub_posits(first, second, a.negative);
    } else {
        return add_posits(first, second, a.negative);
    }
}
