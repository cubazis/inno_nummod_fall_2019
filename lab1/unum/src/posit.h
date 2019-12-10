//
// Created by LinaChasova.
//
//

#include "stdint.h"
#include "math.h"
#include "stdbool.h"
#include "stdio.h"
#include "inttypes.h"

#include "posit_types.h"

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef struct _posit32 {
  POSIT_UTYPE mBits; 
} posit;

bool isZero(posit self);
bool isNar(posit self);
bool isNeg(posit self);

int ss(void);
int rs(posit self);                      
int es(posit self);                       
int fs(posit self);

posit zero(void);
posit one(void);
posit nar(void);

posit neg(posit self);
posit rec(posit self);

posit add(posit a, posit p);
posit sub(posit a, posit p);
posit mul(posit a, posit p);
posit divide(posit a, posit p);

posit power(posit a, posit b);

bool eq(posit a, posit p);
bool gt(posit a, posit p);
bool ge(posit a, posit p);
bool lt(posit a, posit p);
bool le(posit a, posit p);

void setPosit(posit *a, posit p);

float getFloat(posit *self);

// debug
void print(posit self);

int test_function(void);

#endif //UNUM_POSIT_H
