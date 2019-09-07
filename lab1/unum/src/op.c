#include "op.h"
#include "util.h"

unpacked unpackedAdd(unpacked a, unpacked b, bool neg)
{
    unpacked r;

    POSIT_LUTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_LUTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_LUTYPE frac;

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
    r.neg = neg;
    r.frac = LSHIFT(frac, 1);

    return r;
}

unpacked unpackedSub(unpacked a, unpacked b, bool neg)
{
    unpacked r;

    POSIT_UTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_UTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_UTYPE frac;

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
    r.neg = neg;
    r.exp -= CLZ(frac);
    r.frac = LSHIFT(frac, CLZ(frac) + 1);

    return r;
}

unpacked op_add(unpacked a, unpacked b)
{
    if (a.neg == b.neg) {
        return unpackedAdd(a, b, a.neg);
    } else {
        return unpackedSub(a, b, a.neg);
    }
}

unpacked op_mul(unpacked a, unpacked b)
{
    unpacked r;
    POSIT_LUTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_LUTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_UTYPE frac = RSHIFT(afrac * bfrac, POSIT_WIDTH);
    POSIT_STYPE exp = a.exp + b.exp + 1;
    if ((frac & POSIT_MSB) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }
    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);
    return r;
}

unpacked op_div(unpacked a, unpacked b)
{
    unpacked r;
    POSIT_LUTYPE afrac = HIDDEN_BIT(a.frac);
    POSIT_LUTYPE bfrac = HIDDEN_BIT(b.frac);
    POSIT_STYPE exp = a.exp - b.exp;
    if (afrac < bfrac) {
        exp--;
        bfrac = RSHIFT(bfrac, 1);
    }
    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(afrac, POSIT_WIDTH) / bfrac;
    return r;
}

unpacked op_sub(unpacked a, unpacked b)
{
    if (a.neg == b.neg) {
        return unpackedSub(a, b, a.neg);
    } else {
        return unpackedAdd(a, b, a.neg);
    }
}
