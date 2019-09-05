//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#include <stdint.h>
#include <stdio.h>

struct _posit32
{
    uint32_t value;
    int es;
    int regime_bit;
    int useed;
    int sign;
    int useed_power;
    int rs;
    int es_real;
    uint8_t e;
    int fs;
    uint32_t f;
};

typedef struct _posit32 posit;

double getValueInPosit32(posit* posit32);

posit multiply(posit* a, posit* b);

int test_function(void);

#endif //UNUM_POSIT_H
