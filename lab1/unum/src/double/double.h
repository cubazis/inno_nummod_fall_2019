#ifndef POSIT_32_DOUBLE_H
#define POSIT_32_DOUBLE_H

#include <stdint.h>

typedef struct double_unpack {
    uint8_t sign;
    uint64_t exponent;
    uint64_t mantissa;
} double_unpack_t;

#define DOUBLE_SIZE 64
#define DOUBLE_SG_SIZE 1
#define DOUBLE_EX_SIZE 11
#define DOUBLE_MA_SIZE 52

#define DOUBLE_SIGN_MASK 0x8000000000000000
#define DOUBLE_EXP_MASK  0x7FF0000000000000
#define DOUBLE_MAN_MASK  0x000FFFFFFFFFFFFF

#define DOUBLE_EX_OFFSET 1023

#endif //POSIT_32_DOUBLE_H
