//
// Created by arsee on 9/5/2019.
//



#include <stdint.h>

#include "util.h"
#include "posit_constants.h"
#include "converter.h"

int nbits =32;
int es =2;

#include <stdio.h>


uint32_t pack_posit(int neg, int32_t exp, uint32_t frac){


    uint32_t posit;
    uint32_t regbits;
    uint32_t expbits;
    //printf("%d - %d -%d\n",neg,exp,frac);
    int maxexp =POW2(es)*nbits;

    if (exp < -maxexp) {
        exp = -maxexp;
    } else if (exp > maxexp) {
        exp = maxexp;
    }

    int reg = FLOORDIV(exp, POW2(es));
    int ss = 1;
    int rs = MAX(-reg + 1, reg + 2);


    if (ss + rs + es >= nbits && frac >= POSIT_MSB) {
        exp++;
        reg = FLOORDIV(exp, POW2(es));
        rs = MAX(-reg + 1, reg + 2);
    }

    uint32_t exponent = exp - POW2(es) * reg;

    if (reg < 0) {
        regbits = RSHIFT(POSIT_MSB, -reg);
    } else {
        regbits = LMASK(POSIT_MASK, reg + 1);
    }
    expbits = LMASK(LSHIFT(exponent, POSIT_WIDTH - es), es);

    posit = frac;
    posit = expbits | RSHIFT(posit, es);
    posit = regbits | RSHIFT(posit, rs);
    posit = RSHIFT(posit, ss);

    if (neg) {
        return util_neg(posit, nbits);
    } else {
        return LMASK(posit, nbits);
    }

}

struct unpacked_posit unpack_posit(uint32_t p){
    struct unpacked_posit unp;

    int neg = util_is_neg(p);
    if (neg) {
        p = util_neg(p, nbits);
    }

    int ss = 1;
    int rs = util_rs(p, nbits);

    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss) | 1); // summ LSB to compensate for sign bit

    int reg = (lz == 0 ? lo - 1 : -lz);
    uint32_t exp = RSHIFT(LSHIFT(p, ss + rs), POSIT_WIDTH - es);

    unp.negative = neg;
    unp.exp = POW2(es) * reg + exp;
    unp.frac = LSHIFT(p, ss + rs + es);

    return unp;
}

uint32_t double_to_posit(double d)
{
    int bias = 1023;

    union {
        double d;
        uint64_t u;
    } un;
    un.d =d;
    int neg = RSHIFT(un.u, 63);
    int32_t exp = (RSHIFT(un.u, 52) & 0x7FF) - bias;
    uint32_t  frac = RSHIFT(LSHIFT(un.u, 12), 64 - POSIT_WIDTH);


    if (exp == -bias) {

        exp -= CLZ(frac);
        frac = LSHIFT(frac, CLZ(frac) + 1);
    }

    return pack_posit(neg,exp,frac);
}

double fromPosit(uint32_t posit){
    struct unpacked_posit up = unpack_posit(posit);
    int fexp = up.exp + 1023;

    // left aligned
    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        // overflow, set maximum value
        fexpbits = LSHIFT((uint64_t)2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        // underflow, pack as denormal
        fexpbits = 0;

        ffracbits = LSHIFT((uint64_t)(POSIT_MSB | RSHIFT(up.frac, 1)), 64 - POSIT_WIDTH);

        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((uint64_t)(fexp & 0x7FF), 53);

        ffracbits = LSHIFT((uint64_t)up.frac, 64 - POSIT_WIDTH);

    }

    union {
        double f;
        uint64_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((uint64_t)up.negative, 63) | RSHIFT(un.u, 1);

    // don't underflow to zero
    if (LSHIFT(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

