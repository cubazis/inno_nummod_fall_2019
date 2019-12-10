#include "pack.h"
#include "util.h"

POSIT_UTYPE pack_posit(unpacked up)
{
    POSIT_UTYPE p;
    POSIT_UTYPE regbits;
    POSIT_UTYPE expbits;

    // handle underflow and overflow.
    // in either case, exponent and fraction bits will disappear.
    int maxexp = POW2(ES) * (NBITS - 2);
    if (up.exp < -maxexp) {
        up.exp = -maxexp;
    } else if (up.exp > maxexp) {
        up.exp = maxexp;
    }

    int reg = FLOORDIV(up.exp, POW2(ES));
    int ss = util_ss();
    int rs = MAX(-reg + 1, reg + 2);

    if (ss + rs + ES >= NBITS && up.frac >= POSIT_MSB) {
        up.exp++;
        reg = FLOORDIV(up.exp, POW2(ES));
        rs = MAX(-reg + 1, reg + 2);
    }

    POSIT_UTYPE exp = up.exp - POW2(ES) * reg;

    if (reg < 0) {
        regbits = RSHIFT(POSIT_MSB, -reg);
    } else {
        regbits = LMASK(POSIT_MASK, reg + 1);
    }
    expbits = LMASK(LSHIFT(exp, POSIT_WIDTH - ES), ES);

    p = up.frac;
    p = expbits | RSHIFT(p, ES);
    p = regbits | RSHIFT(p, rs);
    p = RSHIFT(p, ss);

    if (up.neg) {
        return util_neg(p);
    } else {
        return LMASK(p, NBITS);
    }
}

float pack_float(unpacked up)
{
    int fexp = up.exp + 127; 

    // left aligned
    uint32_t fexpbits;
    uint32_t ffracbits;

    if (fexp > 254) {
        // overflow, set maximum value
        fexpbits = LSHIFT(254, 24);
        ffracbits = -1;
    } else if (fexp < 1) {
        // underflow, pack as denormal
        fexpbits = 0;

        ffracbits = (uint32_t)(POSIT_MSB | RSHIFT(up.frac, 1));
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT(fexp & 0xFF, 24);
        ffracbits = LSHIFT((uint32_t)up.frac, 32 - POSIT_WIDTH);
    }

    union {
        float f;
        uint32_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 8);
    un.u = LSHIFT((uint32_t)up.neg, 31) | RSHIFT(un.u, 1);

    // don't underflow to zero
    if (LSHIFT(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

unpacked unpack_posit(POSIT_UTYPE p)
{
    unpacked up;

    bool neg = util_is_neg(p);
    if (neg) {
        p = util_neg(p);
    }

    int ss = util_ss();
    int rs = util_rs(p);

    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss) | 1); // add LSB to compensate for sign bit

    int reg = (lz == 0 ? lo - 1 : -lz);
    POSIT_UTYPE exp = RSHIFT(LSHIFT(p, ss + rs), NBITS - ES);

    up.neg = neg;
    up.exp = POW2(ES) * reg + exp;
    up.frac = LSHIFT(p, ss + rs + ES);

    return up;
}
