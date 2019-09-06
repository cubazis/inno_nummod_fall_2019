//
// Created by cubazis on 18.08.2019.
//

#include "posit.h"



#include "posit.h"
#include <math.h>
//#include "posit_types.h"


posit32 posit32_init_fromparts(uint32_t reprBits){
    posit32 instance;
    instance.reprBits = reprBits;
    return instance;
}



struct float_struct divide_float(float f){
    struct float_struct up;
    int bias = 127;
    union {
        float f;
        uint32_t u;
    } un;

    un.f = f;

    up.neg = RSHIFT(un.u, 31);
    up.exp = (RSHIFT(un.u, 23) & 0xFF) - bias;
    up.frac = RSHIFT(LSHIFT(un.u, 9), 32 - nBits);

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}
posit32 construct_from_fl(struct float_struct up){
    uint32_t p;
    uint32_t regime;
    uint32_t exponent;

    // handle underflow and overflow.
    // in either case, exponent and fraction bits will disappear.
    int maxexp = POW2(Es) * (nBits - 2);
    if (up.exp < -maxexp) {
        up.exp = -maxexp;
    } else if (up.exp > maxexp) {
        up.exp = maxexp;
    }

    int reg = FLOORDIV(up.exp, POW2(Es));
    int ss = 1;
    int rs = MAX(-reg + 1, reg + 2);

    if (ss + rs + Es >= nBits && up.frac >= ((uint32_t)0x80000000)) {
        up.exp++;
        reg = FLOORDIV(up.exp, POW2(Es));
        rs = MAX(-reg + 1, reg + 2);
    }

    uint32_t exp = up.exp - POW2(Es) * reg;

    if (reg < 0) {
        regime = RSHIFT(((uint32_t)0x80000000), -reg);
    } else {
        regime = LMASK(((uint32_t)0xFFFFFFFF), reg + 1);

    }
    exponent = LMASK(LSHIFT(exp, nBits - Es), Es);

    p = up.frac;
    p = exponent | RSHIFT(p, Es);
    p = regime | RSHIFT(p, rs);
    p = RSHIFT(p, ss);

    uint32_t pos_num;
    if (up.neg) {
        pos_num =  LMASK(-LMASK(p, nBits), nBits);;
    } else {
        pos_num =  LMASK(p, nBits);
    }
    return posit32_init_fromparts(pos_num);
}

posit32 posit32_init(float val){
    posit32 instance;

    switch (fpclassify(val)) {
        case FP_INFINITE:
        case FP_NAN:
            instance.reprBits = ((uint32_t)0x80000000);
            break;
        case FP_ZERO:
            instance.reprBits = ((uint32_t)0x00000000);
            break;
        default:
            instance = construct_from_fl(divide_float(val));
            break;
    }
    return instance;
}


posit32 posit32_addition(posit32 q1, posit32 q2){
    double a = getDouble(q1);
    double b = getDouble(q2);

    return posit_init_double(a+b);
}

posit32 posit32_subtraction(posit32 q1, posit32 q2){
    double a = getDouble(q1);
    double b = getDouble(q2);

    return posit_init_double(a-b);
}

posit32 posit32_multiplication(posit32 q1, posit32 q2){
    double a = getFloatRepr_new(q1);
    double b = getFloatRepr_new(q2);
    double c = a*b;
    return posit_init_double(c);
}
posit32 posit32_division(posit32 q1, posit32 q2){
    double a = getDouble(q1);
    double b = getDouble(q2);

    return posit_init_double(a/b);
}


int util_conv_func(uint32_t p, int nbits)
{
    int ss = 1;
    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss));
    int rs = MAX(lz, lo) + 1;

    return MIN(rs, nbits - ss);
}
double pack_double_new(struct float_struct up)
{
    int fexp = up.exp + 1023;

    // left aligned
    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        // overflow, set maximum value
        fexpbits = LSHIFT((uint64_t)2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        // underflow, pack as denormal
        fexpbits = 0;

        ffracbits = LSHIFT((uint64_t)(((uint32_t)0x80000000) | RSHIFT(up.frac, 1)), 64 - nBits);

        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((uint64_t)(fexp & 0x7FF), 53);

        ffracbits = LSHIFT((uint64_t)up.frac, 64 - nBits);

    }

    union {
        double f;
        uint64_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((uint64_t)up.neg, 63) | RSHIFT(un.u, 1);

    // don't underflow to zero
    if (LSHIFT(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}


float pack_float_new(struct float_struct up)
{
    int fexp = up.exp + 127;

    // left aligned
    uint32_t fexpbits;
    uint32_t ffracbits;

    if (fexp > 254) {
        // overflow, set maximum value
        fexpbits = LSHIFT(254, 24);
        ffracbits = -1;
    } else if (fexp < 1) {
        // underflow, pack as denormal
        fexpbits = 0;
        ffracbits = LSHIFT((uint32_t)(((uint32_t)0x80000000) | RSHIFT(up.frac, 1)), 32 - nBits);
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT(fexp & 0xFF, 24);
        ffracbits = LSHIFT((uint32_t)up.frac, 32 - nBits);
    }

    union {
        float f;
        uint32_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 8);
    un.u = LSHIFT((uint32_t)up.neg, 31) | RSHIFT(un.u, 1);

    // don't underflow to zero
    if (LSHIFT(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

struct float_struct unpack_posit_new(uint32_t p)
{


    struct float_struct up;

    bool neg = (int32_t)p < 0 && !(p == ((uint32_t)0x80000000));


    if (neg) {
        p = LMASK(-LMASK(p, 32), 32);
    }

    int ss = 1;
    int rs = util_conv_func(p, nBits);

    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss) | 1); // add LSB to compensate for sign bit

    int reg = (lz == 0 ? lo - 1 : -lz);
    uint32_t exp = RSHIFT(LSHIFT(p, ss + rs), nBits - Es);


    up.neg = neg;
    up.exp = POW2(Es) * reg + exp;
    up.frac = LSHIFT(p, ss + rs + Es);

    return up;
}

float getFloatRepr_new(posit32 p){

    if (isZero(p)) {
        return 0.0;
    } else if (isNar(p)) {
        return 0.0 / 0.0;}

    return pack_float_new(unpack_posit_new(p.reprBits));
}


bool isZero(posit32 a){
    return a.reprBits == ((uint32_t)0x00000000);

}


bool isNar(posit32 a){
    return a.reprBits == ((uint32_t)0x80000000);

}

double getDouble(posit32 a)
{

    if (isZero(a)) {
        return 0.0;
    } else if (isNar(a)) {
        return 0.0 / 0.0;
    }
    return pack_double_new(unpack_posit_new(a.reprBits));
}


//Posit convertion algorithm from posit documentation, works only for posit(32, 2)
posit32 convertion(float a){
    int i;
    uint32_t p;
    float y = fabsl(a);
//    float j = y;
    uint32_t e = pow(2, Es-1);
    uint32_t useed = pow(2, pow(2, Es));
//    int puk = powl(2, Es);

    if (y>=1){
        p = 1;
        i = 2;

        while (y>=useed && i<nBits){
//            uint32_t iop = LSHIFT(p, 1);
            p = LSHIFT(p, 1) + 1;
            y/=(float)useed;
            i++;
        }
        p = LSHIFT(p, 1);
        i++;

    }

    else {
        p = 0;
        i = 1;
        while (y < 1 && i <= nBits) {

            y *= (float)useed;
            i++;
        }
        if (i >= nBits) {
            p = 2;
            i = nBits + 1;
        }
        else {
            p = 1;
            i++;
        }
    }


    while (e > 1/2 && i <= nBits) {
        p  = LSHIFT(p, 1);
        if (y >= pow(2, e)) {
            y /= (float)pow(2, e);
            p++;
        }
        e /= 2;
        i++;
    }
    y = fmod(y, 1);

    while (y > 0 && i <= nBits) {
        y *= (float)2;
        p = LSHIFT(p, 1) + floor( y );
        y -= floor( y );
        i++;
    }

    p *= pow(2,nBits+1-i );
    i++;
    i = p & 1;
    p = floor( p / 2 );
    if (i == 0) {
//#$p = $p; # closer to lower value
    }
    else if (y == 1 || y == 0) {
        p = p + ( p & 1 );
    }
    else {
        p = p + 1;
    }

    int npat   = pow(2, nBits);
    if (a<0){
        uint32_t kik = (npat-p)%npat;
        return posit32_init_fromparts(kik);
    }
    else{
        uint32_t kik = (p)%npat;
        return posit32_init_fromparts(kik);
    }
}

struct float_struct add(struct float_struct a, struct float_struct b, bool neg)
{
    struct float_struct r;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    uint64_t frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (RSHIFT(frac, nBits) != 0) {
        r.exp++;
        frac = RSHIFT(frac, 1);
    }

    r.neg = neg;
    r.frac = LSHIFT(frac, 1);

    return r;
}

struct float_struct sub(struct float_struct a, struct float_struct b, bool neg)
{
    struct float_struct r;

    uint32_t afrac = HIDDEN_BIT(a.frac);
    uint32_t bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac;

    if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
        r.exp = a.exp;
        bfrac = RSHIFT(bfrac, a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        neg = !neg;
        r.exp = b.exp;
        afrac = RSHIFT(afrac, b.exp - a.exp);
        frac = bfrac - afrac;
    }

    r.neg = neg;
    r.exp -= CLZ(frac);
    r.frac = LSHIFT(frac, CLZ(frac) + 1);

    return r;
}


struct float_struct op2_mul_new(struct float_struct a, struct float_struct b)
{
    struct float_struct r;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac = RSHIFT(afrac * bfrac, nBits);
    int32_t exp = a.exp + b.exp + 1;

    if ((frac & ((uint32_t)0x80000000)) == 0) {

        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return r;
}

struct float_struct op2_div_new(struct float_struct a,struct float_struct b)
{
    struct float_struct r;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    int32_t exp = a.exp - b.exp;

    if (afrac < bfrac) {
        exp--;
        bfrac = RSHIFT(bfrac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(afrac, nBits) / bfrac;

    return r;
}

struct float_struct op2_add_new(struct float_struct a, struct float_struct b){

    if (a.neg == b.neg) {
        return add(a, b, a.neg);
    } else {
        return sub(a, b, a.neg);
    }
}

struct float_struct op2_sub_new(struct float_struct a, struct float_struct b){
    if (a.neg == b.neg) {
        return sub(a, b, a.neg);
    } else {
        return add(a, b, a.neg);
    }
}


posit32 mul(posit32 p1, posit32 p2)
{

    struct float_struct aup = unpack_posit_new(p1.reprBits);
    struct float_struct bup = unpack_posit_new(p2.reprBits);
    struct float_struct up = op2_mul_new(aup, bup);

    return construct_from_fl(up);
}

posit32 div32(posit32 p1, posit32 p2){
//{
//if (isNar() || p.isNar() || p.isZero()) {
//return nar();
//} else if (isZero()) {
//return zero();
//}

    struct float_struct aup = unpack_posit_new(p1.reprBits);
    struct float_struct bup = unpack_posit_new(p2.reprBits);
    struct float_struct up = op2_div_new(aup, bup);

    return construct_from_fl(up);
}

posit32 add32(posit32 p1, posit32 p2){

    struct float_struct aup = unpack_posit_new(p1.reprBits);
    struct float_struct bup = unpack_posit_new(p2.reprBits);
    struct float_struct up = op2_add_new(aup, bup);

    return construct_from_fl(up);
}

posit32 sub32(posit32 p1, posit32 p2){


    struct float_struct aup = unpack_posit_new(p1.reprBits);
    struct float_struct bup = unpack_posit_new(p2.reprBits);
    struct float_struct up = op2_sub_new(aup, bup);

    return construct_from_fl(up);
}

struct float_struct divide_double(double f)
{
    struct float_struct up;
    int bias = 1023;

    union {
        double f;
        uint64_t u;
    } un;

    un.f = f;

    up.neg = RSHIFT(un.u, 63);
    up.exp = (RSHIFT(un.u, 52) & 0x7FF) - bias;
    up.frac = RSHIFT(LSHIFT(un.u, 12), 64 - nBits);

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

posit32 posit_init_double(double a){
    posit32 instance;

    switch (fpclassify(a)) {
        case FP_INFINITE:
        case FP_NAN:
            instance.reprBits = ((uint32_t)0x80000000);
            break;
        case FP_ZERO:
            instance.reprBits = ((uint32_t)0x00000000);
            break;
        default:
            instance = construct_from_fl(divide_double(a));
            break;
    }
    return instance;
}


posit32 get_pos_toconvert(long double * num, int quire_i){
    long double maxposit = 1E+10;

    long double a = *num;
    if (a>maxposit && quire_i < q_size-1) {

        posit32 b =  posit_init_double(maxposit);

        a = a/maxposit;
        *num = a;

        return b;
    } else if (a>maxposit && quire_i == q_size-1){
        posit32 c = posit_init_double(a);
        *num = 1;
        return c;
    }
    else {
        posit32 c = posit_init_double(a);
        *num = 1;
        return c;
    }
}

simple_quire pow_large(posit32 a, long double b){

    posit32 quire[q_size];
    simple_quire res;
    long double power = powl(getDouble(a), b);
    for (int i = 0; i<q_size; i++){
        quire[i] = get_pos_toconvert(&power, i);
        res.pos[i] = quire[i];
    }

    return res;

}

long double q_t_d(simple_quire a){

    long double res = 1;

    for (int i = 0; i<q_size; i++){
        res*=((long double)getDouble(a.pos[i]));
    }
    return res;
}


simple_quire negate(simple_quire a){

    simple_quire b = a;
    if (q_size >0){
        b.pos[0] = sub32(posit32_init(0), b.pos[0]);
    }
    return b;

}

int test_function(void)
{
    return 42;
}