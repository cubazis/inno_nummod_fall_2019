#ifndef __POSIT_H
#define __POSIT_H

#include "posit_types.h"

typedef struct _posit32 {

    POSIT_UTYPE mBits;
    //int mNbits;
    //int mEs;
} posit;

bool isZero(posit p);                  // check for 0
bool isNar(posit p);                   // check for NaR
bool isNeg(posit p);                   // check for negative

int ss(void);                       // sign size in bits
int rs(posit p);                       // regime size in bits
int es(posit p);                       // exponent size in bits
int fs(posit p);                       // fraction size in bits

posit zero(void);                   // 0
posit one(void);                    // 1
posit nar(void);                    // NaR

posit neg(posit p);                    // -x
posit rec(posit p);                    // 1 / x

posit add(posit p, posit p2);      // x + p
posit sub(posit p, posit p2);      // x - p
posit mul(posit p, posit p2);      // x * p
posit divide(posit p, posit p2);      // x / p
posit power(posit p, posit p2);

bool eq(posit p, posit p2);        // x == p
bool gt(posit p, posit p2);        // x > p
bool ge(posit p, posit p2);        // x >= p
bool lt(posit p, posit p2);        // x < p
bool le(posit p, posit p2);        // x <= p

void setPosit(posit *p, posit p2);                    // x = p

float getFloat(posit p);               // n = x
double getDouble(posit p);             // n = x

void print(posit p);


int test_function(void);

#endif //UNUM_POSIT_H
