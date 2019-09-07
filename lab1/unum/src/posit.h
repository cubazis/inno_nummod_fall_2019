//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define signP32UI( a ) ((bool) ((uint32_t) (a)>>31))
#define signregP32UI( a ) ((bool) (((uint32_t) (a)>>30) & 0x1))
#define packToP32UI(regime, expA, fracA) ( (uint32_t) regime + (uint32_t) expA + ((uint32_t)(fracA)) )


typedef struct { uint32_t v; } posit;
union ui32_p32 { uint32_t ui; posit p; };
union ui64_double { uint64_t ui; double d; };


posit convertDoubleToP32(double);
double convertP32ToDouble(posit);
posit power(posit num, posit p);
posit p32_add(posit, posit);
posit p32_sub(posit, posit);
posit p32_mul(posit, posit);

int test_function(void);

#endif //UNUM_POSIT_H
