#ifndef POSIT_32_POSIT_H
#define POSIT_32_POSIT_H

#include <stdint.h>

typedef struct posit32 {
    uint32_t value;
} posit32_t;

typedef struct posit32_unpack {
    uint8_t sign; // 0 or 1
    int regime; // regular number
    uint32_t exponent; // regular number
    uint32_t fraction; // fraction bits moved to the left
} posit_unpack_t;

#define POSIT32_SIZE    32
#define POSIT32_U_SIZE  8 * sizeof(uint32_t)
#define POSIT32_ES_SIZE 2

#define POSIT32_ZERO 0x00000000
#define POSIT32_NAR  0x40000000

#define LSB_MASK     0x00000001
#define MSB_MASK     0x80000000
#define FILLED_MASK  0xFFFFFFFF

#endif //POSIT_32_POSIT_H
