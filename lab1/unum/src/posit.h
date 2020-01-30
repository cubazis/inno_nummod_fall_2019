#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef struct _posit32 {
    unsigned int bits;
} posit;

posit toPosit(double a);

double toDouble(posit a);

posit add(posit a, posit b);

posit subtract(posit a, posit b);

posit multiply(posit a, posit b);

posit power(posit a, posit b);

posit negate(posit a);

posit posit_abs(posit a);

short is_less(posit rhs, posit lhs);

#endif