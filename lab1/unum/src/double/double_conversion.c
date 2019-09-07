#include "stdint.h"
#include "double.h"
#include "../utils.h"

double_unpack_t unpack_double(double d) {
    double_unpack_t du;

    union {
        double v;
        uint64_t bits;
    } dr;
    dr.v = d;

    du.sign = RSHIFT(dr.bits, DOUBLE_EX_SIZE + DOUBLE_MA_SIZE);
    du.exponent = RSHIFT(dr.bits & DOUBLE_EXP_MASK, DOUBLE_MA_SIZE);
    du.mantissa = dr.bits & DOUBLE_MAN_MASK;

    return du;
}

double pack_double(double_unpack_t du) {
    union {
        double v;
        uint64_t bits;
    } d;
    d.v = 0;

    d.bits |= LSHIFT((uint64_t) du.sign, DOUBLE_EX_SIZE + DOUBLE_MA_SIZE) & DOUBLE_SIGN_MASK;
    d.bits |= LSHIFT(du.exponent, DOUBLE_MA_SIZE) & DOUBLE_EXP_MASK;
    d.bits |= du.mantissa & DOUBLE_MAN_MASK;

    return d.v;
}
