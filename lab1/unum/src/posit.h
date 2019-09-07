//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef struct _posit32 {
	unsigned int bits;
} posit;

posit from_double(double a);

double to_double(posit a);

posit add(posit a, posit b);

posit subtract(posit a, posit b);

posit multiply(posit a, posit b);

posit divide(posit a, posit b);

posit power(posit a, posit b);

posit negate(posit a);

int test_function(void);

#endif //UNUM_POSIT_H
