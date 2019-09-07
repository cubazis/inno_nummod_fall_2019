//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef unsigned int posit;

posit p32_from_double(double a);

double p32_to_double(posit a);

posit p32_add(posit a, posit b);

posit p32_mul(posit a, posit b);

#endif //UNUM_POSIT_H
