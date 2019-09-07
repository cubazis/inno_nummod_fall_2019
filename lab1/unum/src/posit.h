//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef struct _posit32 {
    unsigned int data;
} posit;

int test_function(void);

posit int2posit(int a);
posit double2posit(double a);
float posit2float(posit a);
posit sum(posit a, posit b);
posit diff(posit a, posit b);
posit multiplication(posit a, posit b);
posit posit_divide(posit a, int b);
posit posit_pow(posit a, int b);
posit diff(posit a, posit b);
int lessthan(posit a, posit b);
int biggerthan(posit a, posit b);
posit negate(posit a);
posit posit_abs(posit a);
int beq(posit a, posit b);

#endif //UNUM_POSIT_H
