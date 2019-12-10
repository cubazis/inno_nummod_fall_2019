#include "posit.h"
#include "util.h"
#include "pack.h"
#include "op.h"
//#include "op2.h"

bool isZero(posit self)
{
    return util_is_zero(self.mBits);
}

bool isNar(posit self)
{
    return util_is_nar(self.mBits);
}

bool isNeg(posit self)
{
    return util_is_neg(self.mBits);
}

int ss()
{ 
    return util_ss();
}

int rs(posit self)
{
    return util_rs(self.mBits);
}

int es(posit self)
{
  return util_es(self.mBits);
}

int fs(posit self)
{
    return util_fs(self.mBits);
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

posit neg(posit self)
{
    if (isNar(self)) {
        return nar();
    }

    posit result = {util_neg(self.mBits)};
    return result;
}

posit rec(posit self)
{
    if (isNar(self) || isZero(self)) {
        return nar();
    }

    return divide(one(), self);
}

posit add(posit self, posit p)
{
    if (isNar(self) || isNar(p)) {
        return nar();
    } else if (isZero(self)) {
        return p;
    } else if (isZero(p)) {
        return self;
    } else if (eq(neg(self), p)) {
        return zero();
    }

    unpacked aup = unpack_posit(self.mBits);
    unpacked bup = unpack_posit(p.mBits);
    unpacked up = op_add(aup, bup);

    posit result = {pack_posit(up)};
    return result;
}

posit sub(posit self, posit p)
{
    if (isNar(self) || isNar(p)) {
        return nar();
    } else if (isZero(self)) {
        return neg(p);
    } else if (isZero(p)) {
        return self;
    } else if (eq(self, p)) {
        return zero();
    }

    unpacked aup = unpack_posit(self.mBits);
    unpacked bup = unpack_posit(p.mBits);
    unpacked up = op_sub(aup, bup);

    posit result = {pack_posit(up)};
    return result;
}

posit mul(posit self, posit p)
{
    if (isNar(self) || isNar(p)) {
        return nar();
    } else if (isZero(self) || isZero(p)) {
        return zero();
    }

    unpacked aup = unpack_posit(self.mBits);
    unpacked bup = unpack_posit(p.mBits);
    unpacked up = op_mul(aup, bup);

    posit result = {pack_posit(up)};
    return result;
}

posit power(posit self, posit p)
{
    posit left, right;
    setPosit(&left, self);
    setPosit(&right, p);

    if (isNar(self)) {
        return nar();
    } else if (isZero(self) & isZero(p)) {
        return nar();
    } else if (isZero(p)) {
        return one();
    } else if (isNeg(p)) {
        setPosit(&left, rec(left));
        setPosit(&right, neg(right));
    } 

    unpacked aup = unpack_posit(left.mBits);
    unpacked bup = unpack_posit(one().mBits);

    int pwr = (int) getFloat(&right);
    for (int i = 0; i < pwr; i++)
    {
        bup = op_mul(aup, bup);
    }

    posit result = {pack_posit(bup)};
    return result;
}

posit divide(posit self, posit p)
{
    if (isNar(self) || isNar(p) || isZero(p)) {
        return nar();
    } else if (isZero(self)) {
        return zero();
    }

    unpacked aup = unpack_posit(self.mBits);
    unpacked bup = unpack_posit(p.mBits);
    unpacked up = op_div(aup, bup);

    posit result = {pack_posit(up)};

    return result;
}

bool eq(posit self, posit p)
{
    return self.mBits == p.mBits;
}

bool gt(posit self, posit p) 
{
    return (POSIT_STYPE)self.mBits > (POSIT_STYPE)p.mBits;
}

bool ge(posit self,posit p) 
{
    return gt(self, p) || eq(self, p);
}

bool lt(posit self,posit p) 
{
    return !gt(self, p) && !eq(self, p);
}

bool le(posit self,posit p) 
{
    return !gt(self, p);
}

void setPosit(posit *a, posit p)
{
    a->mBits = p.mBits;
}

float getFloat(posit *self) 
{
    if (isZero(*self)) {
        return 0.f;
    } else if (isNar(*self)) {
        return 0.f / 0.f;
    }

    return pack_float(unpack_posit(self->mBits));
}

void print(posit self)
{
    posit p = (isNeg(self) ? neg(self) : self);

    printf("{%d, %d} ", NBITS, ES);

    if (isNar(self)) {
        printf("NaR\n");
        return;
    }

    for (int i = NBITS - 1; i >= 0; i--) {
        printf("%d", RSHIFT(self.mBits, i) & 1);
    }

    printf(" -> ");
    printf(isNeg(self) ? "-" : "+");

    for (int i = NBITS - ss() - 1; i >= 0; i--) {
        printf("%d", RSHIFT(p.mBits, i) & 1);

        if (i != 0 &&
            ((i == NBITS - ss() - rs(p)) ||
             (i == NBITS - ss() - rs(p) - ES))) {
            printf(" ");
        }
    }

    printf(" = %f\n", getFloat(&self));
}

int test_function(void)
{
	return 42;
}
