#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

typedef struct _posit32 {
	/** implement your code **/

    uint32_t bits;
} posit;	

posit fromDouble(double a);

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