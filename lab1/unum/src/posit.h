//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

#include <stdint.h>
#include <stdio.h>

typedef struct _posit32
{
    uint32_t value;
} posit;

double getValueInPosit32(posit posit32);

int test_function(void);

#endif //UNUM_POSIT_H
