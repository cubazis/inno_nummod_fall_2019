//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef struct _posit32 {
    unsigned int bits;
} posit;

posit positFromDouble(double a);

double toDouble(posit a);

posit add(posit a, posit b);

posit sub(posit a, posit b);

posit multiply(posit a, posit b);

posit divide(posit a, posit b);

posit power(posit a, posit b);

posit negate(posit a);

posit p_abs(posit ap);

short is_less(posit rhs, posit lhs);

#endif //UNUM_POSIT_H
