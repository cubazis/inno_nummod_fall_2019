#include "posit.h"
#include "util.h"
#include "pack.h"
#include "op.h"

#include "stdio.h"
#include "math.h"

bool isZero(posit p) 
{
    return util_is_zero(p.mBits);
}

bool isNar(posit p) 
{
    return util_is_nar(p.mBits);
}

bool isNeg(posit p) 
{
    return util_is_neg(p.mBits);
}

int ss() 
{
    return util_ss();
}

int rs(posit p) 
{
    return util_rs(p.mBits);
}

int es(posit p) 
{
    return util_es(p.mBits);
}

int fs(posit p) 
{
    return util_fs(p.mBits);
}

posit zero() 
{
    posit result = {POSIT_ZERO};
    return result;
}

posit one() 
{
    posit result = {POSIT_ONE};
    return result; 
}

posit nar() 
{
    posit result = {POSIT_NAR};
    return result; 
}

posit neg(posit p) 
{
    if (isNar(p)) {
        return nar();
    }
    posit result = {util_neg(p.mBits)};
    return result;
}

posit rec(posit p) 
{
    if (isNar(p) || isZero(p)) {
        return nar();
    }

    return divide(one(), p);
}

posit add(posit p, posit p2) 
{
    if (isNar(p) || isNar(p2)) {
        return nar();
    } else if (isZero(p)) {
        return p;
    } else if (isZero(p2)) {
        return p2;
    } else if (eq(neg(p), p2)) {
        return zero();
    }

    unfold aup = unpack_posit(p.mBits);
    unfold bup = unpack_posit(p2.mBits);
    unfold up = op_add(aup, bup);

    posit result = {pack_posit(up)};
    return result;
}

posit sub(posit p, posit p2) 
{
    if (isNar(p) || isNar(p2)) {
        return nar();
    } else if (isZero(p2)) {
        return neg(p2);
    } else if (isZero(p)) {
        return p;
    } else if (eq(p, p2)) {
        return zero();
    }

    unfold aup = unpack_posit(p.mBits);
    unfold bup = unpack_posit(p2.mBits);
    unfold up = op_sub(aup, bup);

    posit result = {pack_posit(up)};
    return result;
}

posit mul(posit p, posit p2) 
{
    if (isNar(p) || isNar(p2)) {
        return nar();
    } else if (isZero(p) || isZero(p2)) {
        return zero();
    }

    unfold aup = unpack_posit(p.mBits);
    unfold bup = unpack_posit(p2.mBits);
    unfold up = op_mul(aup, bup);

    posit result = {pack_posit(up)};
    return result; 
}

posit power(posit p, posit p2)
{
    
    if (isNar(p) || isNar(p2)) {
        return nar();
    } else if (isZero(p) && isZero(p2)){
        return nar();
    } else if (isZero(p2)){
        return one();
    } else if (isZero(p)){
        return zero();
    } else if (isNeg(p2)){
        return divide(one(), power(p, neg(p2)));
    }    
    
    posit result = one();
    int limit = (int) getDouble(p2);
    int i = 0;
    for(i = 0; i < limit; i++){
        result = mul(result, p);
    }
    return result;
}

posit divide(posit p, posit p2) 
{
    if (isNar(p) || isNar(p2) || isZero(p2)) {
        return nar();
    } else if (isZero(p)) {
        return zero();
    }

    unfold aup = unpack_posit(p.mBits);
    unfold bup = unpack_posit(p2.mBits);
    unfold up = op_div(aup, bup);

    posit result = {pack_posit(up)};
    return result; 
}

float getFloat(posit p) 
{
    if (isZero(p)) {
        return 0.f;
    } else if (isNar(p)) {
        return 0.f / 0.f;
    }

    return pack_float(unpack_posit(p.mBits));
}

double getDouble(posit p) 
{
    if (isZero(p)) {
        return 0.0;
    } else if (isNar(p)) {
        return 0.0 / 0.0;
    }

    return pack_double(unpack_posit(p.mBits));
}

void print(posit p)
{
    posit pOther = (isNeg(p) ? neg(p) : p);

    if (isNar(pOther)) {
        printf("NaR\n");
        return;
    }

    for (int i = NBITS - 1; i >= 0; i--) {
        printf("%d", RSHIFT(pOther.mBits, i) & 1);
    }

    printf(" -> ");
    printf(isNeg(pOther) ? "-" : "+");

    for (int i = NBITS - ss() - 1; i >= 0; i--) {
        printf("%d", RSHIFT(pOther.mBits, i) & 1);

        if (i != 0 &&
            ((i == NBITS - ss() - rs(pOther)) ||
             (i == NBITS - ss() - rs(pOther) - ES))) {
            printf(" ");
        }
    }

    printf(" = %lg\n", getDouble(p));
}

bool eq(posit p, posit p2) 
{
    return p.mBits == p2.mBits;
}

bool gt(posit p, posit p2) 
{
    return (POSIT_STYPE)p.mBits > (POSIT_STYPE)p2.mBits;
}

bool ge(posit p, posit p2) 
{
    return gt(p, p2) || eq(p, p2);
}

bool lt(posit p, posit p2) 
{
    return !gt(p, p2) && !eq(p, p2);
}

bool le(posit p, posit p2) 
{
    return !gt(p, p2);
}

void setPosit(posit *p, posit p2)
{
    p->mBits = p2.mBits;
}

int test_function(void)
{
    return 42;
}
