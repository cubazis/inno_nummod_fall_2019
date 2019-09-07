#include <stdio.h>
#include "stdint.h"

void print_num(uint32_t num) {
    printf("%i\n", num);
}

void print_bits(long num) {
    int count = sizeof(long) * 8;

    uint64_t mask = 0x01 << (count - 1);
    for (int bit = 0; bit < count; bit++) {
        printf("%li", ((num & mask) >> (count - bit - 1)));

        if ((bit + 1) % 4 == 0 && bit != count - 1) {
            if((bit + 1) % 32 == 0) {
                printf(" | ");
            } else {
                printf("_");
            }
        }

        mask >>= 1;
    }

    printf("\n");
}

void print_double_bits(double num) {
    union {
        double v;
        uint64_t bits;
    } dp;

    dp.v = num;
    print_bits(dp.bits);
}
