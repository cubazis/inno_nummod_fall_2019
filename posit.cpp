#include "posit.h"
#include "packing_posit.h"
#include "operations.h"

#include <cstdio>
#include <cmath>

using namespace std;


bool util_is_neg(utype p)
{
    return (POSIT_STYPE)p < 0 && (p != nar_const);
}

int util_rs(utype p, int nbits)
{
    int lz = CLZ(shift_left(p, 1));
    int lo = CLZ(shift_left(~p, 1));
    int rs = MAX(lz, lo) + 1;
    return MIN(rs, nbits - 1);
}

int util_es(utype p, int nbits, int es)
{
    int rs = util_rs(p, nbits);
    return MIN(MAX(nbits - 1 - rs, 0), es);
}

int util_fs(utype p, int nbits, int es)
{
    int rs = util_rs(p, nbits);
    return MAX(nbits - 1 - rs - es, 0);
}

utype util_neg(utype p, int nbits)
{
    return LMASK(-LMASK(p, nbits), nbits);
}


Posit::Posit(utype bits, int nbits, int es) :
    mBits(bits),
    mNbits(nbits),
    mEs(es)
{
}

Posit::Posit(int nbits, int es) :
    Posit(zero_const, nbits, es)
{
}

bool Posit::isZero() const
{
    return mBits == zero_const;
}

bool Posit::isNar() const
{
    return mBits == nar_const;
}

bool Posit::isNeg() const
{
    return util_is_neg(mBits);
}

int Posit::nbits() const
{
    return mNbits;
}

int Posit::rs() const
{
    return util_rs(mBits, mNbits);
}

int Posit::es() const
{
    return util_es(mBits, mNbits, mEs);
}

int Posit::fs() const
{
    return util_fs(mBits, mNbits, mEs);
}

Posit Posit::zero() const
{
    return Posit(zero_const, mNbits, mEs);
}

Posit Posit::one() const
{
    return Posit(one_const, mNbits, mEs);
}

Posit Posit::nar() const
{
    return Posit(nar_const, mNbits, mEs);
}

Posit Posit::neg() const
{
    if (isNar()) {
        return nar();
    }
    return Posit(util_neg(mBits, mNbits), mNbits, mEs);
}

Posit Posit::rec() const
{
    if (isNar() || isZero()) {
        return nar();
    }
    return one().div(*this);
}

Posit Posit::sqrt() const
{
    if (isNar() || isNeg()) {
        return nar();
    } else if (isZero()) {
        return zero();
    }
    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t up = posit_sqrt(aup);
    return Posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

Posit Posit::add(const Posit& p) const
{
    if (isNar() || p.isNar()) {
        return nar();
    } else if (isZero()) {
        return p;
    } else if (p.isZero()) {
        return *this;
    } else if (neg().equal(p)) {
        return zero();
    }

    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t bup = unpack_posit(p.mBits, p.mNbits, p.mEs);
    unpacked_t up = op2_add(aup, bup);

    return Posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

Posit Posit::sub(const Posit& p) const
{
    if (isNar() || p.isNar()) {
        return nar();
    } else if (isZero()) {
        return p.neg();
    } else if (p.isZero()) {
        return *this;
    } else if (equal(p)) {
        return zero();
    }

    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t bup = unpack_posit(p.mBits, p.mNbits, p.mEs);
    unpacked_t up = op2_sub(aup, bup);

    return Posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

Posit Posit::mul(const Posit& p) const
{
    if (isNar() || p.isNar()) {
        return nar();
    } else if (isZero() || p.isZero()) {
        return zero();
    }

    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t bup = unpack_posit(p.mBits, p.mNbits, p.mEs);
    unpacked_t up = op2_mul(aup, bup);
    return Posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

Posit Posit::div(const Posit& p) const
{
    if (isNar() || p.isNar() || p.isZero()) {
        return nar();
    } else if (isZero()) {
        return zero();
    }

    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t bup = unpack_posit(p.mBits, p.mNbits, p.mEs);
    unpacked_t up = op2_div(aup, bup);

    return Posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

bool Posit::equal(const Posit& p) const
{
    return mBits == p.mBits;
}

bool Posit::greater(const Posit& p) const
{
    return (POSIT_STYPE)mBits > (POSIT_STYPE)p.mBits;
}

bool Posit::greaterOrEqual(const Posit& p) const
{
    return greater(p) || equal(p);
}

bool Posit::less(const Posit& p) const
{
    return !greater(p) && !equal(p);
}

bool Posit::lessOrEqual(const Posit& p) const
{
    return !greater(p);
}

void Posit::set(Posit p)
{
    mBits = pack_posit(unpack_posit(p.mBits, p.mNbits, p.mEs), mNbits, mEs);
}

void Posit::set(float n)
{
    switch (fpclassify(n)) {
    case FP_INFINITE:
    case FP_NAN:
        mBits = nar_const;
        break;
    case FP_ZERO:
        mBits = zero_const;
        break;
    default:
        mBits = pack_posit(unpack_float(n), mNbits, mEs);
        break;
    }
}

void Posit::set(double n)
{
    switch (fpclassify(n)) {
    case FP_INFINITE:
    case FP_NAN:
        mBits = nar_const;
        break;
    case FP_ZERO:
        mBits = zero_const;
        break;
    default:
        mBits = pack_posit(unpack_double(n), mNbits, mEs);
        break;
    }
}

float Posit::getFloat() const
{
    if (isZero()) {
        return 0.f;
    } else if (isNar()) {
        return 0.f / 0.f;
    }

    return pack_float(unpack_posit(mBits, mNbits, mEs));
}

double Posit::getDouble() const
{
    if (isZero()) {
        return 0.0;
    } else if (isNar()) {
        return 0.0 / 0.0;
    }
    return pack_double(unpack_posit(mBits, mNbits, mEs));
}

Posit32::Posit32() :
    Posit(32, 2){}

Posit32::Posit32(Posit v) :
    Posit32()
{
    set(v);
}

Posit32::Posit32(float v) :
    Posit32()
{
    set(v);
}

Posit32::Posit32(double v) :
    Posit32()
{
    set(v);
}

Posit operator+(const Posit& a, const Posit& b)
{
    return a.add(b);
}

Posit operator-(const Posit& a, const Posit& b)
{
    return a.sub(b);
}

Posit operator*(const Posit& a, const Posit& b)
{
    return a.mul(b);
}

Posit operator/(const Posit& a, const Posit& b)
{
    return a.div(b);
}

Posit operator-(const Posit& a)
{
    return a.neg();
}

bool operator<(const Posit&a , const Posit& b)
{
    return a.less(b);
}

bool operator<=(const Posit&a , const Posit& b)
{
    return a.lessOrEqual(b);
}

bool operator>(const Posit&a , const Posit& b)
{
    return a.greater(b);
}

bool operator>=(const Posit&a , const Posit& b)
{
    return a.greaterOrEqual(b);
}
bool operator==(const Posit&a , const Posit& b)
{
    return a.equal(b);
}

bool operator!=(const Posit&a , const Posit& b)
{
    return !a.equal(b);
}

static struct unpacked_t half(struct unpacked_t a)
{
    struct unpacked_t r = a;
    r.exp--;
    return r;
}

struct unpacked_t posit_sqrt(struct unpacked_t a)
{
    struct unpacked_t r = a;
    r.exp /= 2;

    for (int i = 0; i < 100; i++) {
        struct unpacked_t rn;
        rn = half(op2_add(r, op2_div(a, r)));

        if (rn.exp == r.exp && rn.frac == r.frac) {
            break;
        }

        r = rn;
    }
    return r;
}

static struct unpacked_t add(struct unpacked_t a, struct unpacked_t b, bool neg)
{
    struct unpacked_t r;

    lutype afrac = HIDDEN_BIT(a.frac);
    lutype bfrac = HIDDEN_BIT(b.frac);
    lutype frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = shift_right(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = shift_right(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (shift_right(frac, POSIT_WIDTH) != 0) {
        r.exp++;
        frac = shift_right(frac, 1);
    }

    r.neg = neg;
    r.frac = shift_left(frac, 1);

    return r;
}

static struct unpacked_t sub(struct unpacked_t a, struct unpacked_t b, bool neg)
{
    struct unpacked_t r;

    utype afrac = HIDDEN_BIT(a.frac);
    utype bfrac = HIDDEN_BIT(b.frac);
    utype frac;

    if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
        r.exp = a.exp;
        bfrac = shift_right(bfrac, a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        neg = !neg;
        r.exp = b.exp;
        afrac = shift_right(afrac, b.exp - a.exp);
        frac = bfrac - afrac;
    }

    r.neg = neg;
    r.exp -= CLZ(frac);
    r.frac = shift_left(frac, CLZ(frac) + 1);

    return r;
}

struct unpacked_t op2_mul(struct unpacked_t a, struct unpacked_t b)
{
    struct unpacked_t r;

    lutype afrac = HIDDEN_BIT(a.frac);
    lutype bfrac = HIDDEN_BIT(b.frac);
    utype frac = shift_right(afrac * bfrac, POSIT_WIDTH);
    POSIT_STYPE exp = a.exp + b.exp + 1;

    if ((frac & msb_const) == 0) {
        exp--;
        frac = shift_left(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = shift_left(frac, 1);

    return r;
}

struct unpacked_t op2_div(struct unpacked_t a, struct unpacked_t b)
{
    struct unpacked_t r;

    lutype afrac = HIDDEN_BIT(a.frac);
    lutype bfrac = HIDDEN_BIT(b.frac);
    POSIT_STYPE exp = a.exp - b.exp;

    if (afrac < bfrac) {
        exp--;
        bfrac = shift_right(bfrac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = shift_left(afrac, POSIT_WIDTH) / bfrac;

    return r;
}

struct unpacked_t op2_add(struct unpacked_t a, struct unpacked_t b)
{
    if (a.neg == b.neg) {
        return add(a, b, a.neg);
    } else {
        return sub(a, b, a.neg);
    }
}

struct unpacked_t op2_sub(struct unpacked_t a, struct unpacked_t b)
{
    if (a.neg == b.neg) {
        return sub(a, b, a.neg);
    } else {
        return add(a, b, a.neg);
    }
}

utype pack_posit(struct unpacked_t up, int nbits, int es)
{
    utype p;
    utype regbits;
    utype expbits;

    int maxexp = x2(es) * (nbits - 2);
    if (up.exp < -maxexp) {
        up.exp = -maxexp;
    } else if (up.exp > maxexp) {
        up.exp = maxexp;
    }

    int reg = floor_div(up.exp, x2(es));
    int rs = MAX(-reg + 1, reg + 2);

    if (1 + rs + es >= nbits && up.frac >= msb_const) {
        up.exp++;
        reg = floor_div(up.exp, x2(es));
        rs = MAX(-reg + 1, reg + 2);
    }

    utype exp = up.exp - x2(es) * reg;

    if (reg < 0) {
        regbits = shift_right(msb_const, -reg);
    } else {
        regbits = LMASK(posit_mask, reg + 1);
    }
    expbits = LMASK(shift_left(exp, POSIT_WIDTH - es), es);

    p = up.frac;
    p = expbits | shift_right(p, es);
    p = regbits | shift_right(p, rs);
    p = shift_right(p, 1);

    if (up.neg) {
        return util_neg(p, nbits);
    } else {
        return LMASK(p, nbits);
    }
}

float pack_float(struct unpacked_t up)
{
    int fexp = up.exp + 127;

    uint32_t fexpbits;
    uint32_t ffracbits;

    if (fexp > 254) {
        fexpbits = shift_left(254, 24);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
#if POSIT_WIDTH <= 32
        ffracbits = shift_left((uint32_t)(msb_const | shift_right(up.frac, 1)), 32 - POSIT_WIDTH);
#else
        ffracbits = shift_right(msb_const | shift_right(up.frac, 1), POSIT_WIDTH - 32);
#endif
        ffracbits = shift_right(ffracbits, -fexp);
    } else {
        fexpbits = shift_left(fexp & 0xFF, 24);
#if POSIT_WIDTH <= 32
        ffracbits = shift_left((uint32_t)up.frac, 32 - POSIT_WIDTH);
#else
        ffracbits = shift_right(up.frac, POSIT_WIDTH - 32);
#endif
    }

    union {
        float f;
        uint32_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | shift_right(un.u, 8);
    un.u = shift_left((uint32_t)up.neg, 31) | shift_right(un.u, 1);

    if (shift_left(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

double pack_double(struct unpacked_t up)
{
    int fexp = up.exp + 1023;

    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        fexpbits = shift_left((uint64_t)2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
#if POSIT_WIDTH <= 64
        ffracbits = shift_left((uint64_t)(msb_const | shift_right(up.frac, 1)), 64 - POSIT_WIDTH);
#else
        ffracbits = shift_right(msb_const | shift_right(up.frac, 1), POSIT_WIDTH - 64);
#endif
        ffracbits = shift_right(ffracbits, -fexp);
    } else {
        fexpbits = shift_left((uint64_t)(fexp & 0x7FF), 53);
#if POSIT_WIDTH <= 64
        ffracbits = shift_left((uint64_t)up.frac, 64 - POSIT_WIDTH);
#else
        ffracbits = shift_right(up.frac, POSIT_WIDTH - 64);
#endif
    }

    union {
        double f;
        uint64_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | shift_right(un.u, 11);
    un.u = shift_left((uint64_t)up.neg, 63) | shift_right(un.u, 1);

    if (shift_left(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

struct unpacked_t unpack_posit(utype p, int nbits, int es)
{
    struct unpacked_t up;

    bool neg = util_is_neg(p);
    if (neg) {
        p = util_neg(p, nbits);
    }

    int rs = util_rs(p, nbits);

    int lz = CLZ(shift_left(p, 1));
    int lo = CLZ(shift_left(~p, 1) | 1); 

    int reg = (lz == 0 ? lo - 1 : -lz);
    utype exp = shift_right(shift_left(p, 1 + rs), POSIT_WIDTH - es);

    up.neg = neg;
    up.exp = x2(es) * reg + exp;
    up.frac = shift_left(p, 1 + rs + es);

    return up;
}

struct unpacked_t unpack_float(float f)
{
    struct unpacked_t up;
    int bias = 127;

    union {
        float f;
        uint32_t u;
    } un;

    un.f = f;

    up.neg = shift_right(un.u, 31);
    up.exp = (shift_right(un.u, 23) & 0xFF) - bias;
#if POSIT_WIDTH <= 32
    up.frac = shift_right(shift_left(un.u, 9), 32 - POSIT_WIDTH);
#else
    up.frac = shift_left((utype)un.u, POSIT_WIDTH - 32 + 9);
#endif

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = shift_left(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

struct unpacked_t unpack_double(double f)
{
    struct unpacked_t up;
    int bias = 1023;

    union {
        double f;
        uint64_t u;
    } un;

    un.f = f;

    up.neg = shift_right(un.u, 63);
    up.exp = (shift_right(un.u, 52) & 0x7FF) - bias;
#if POSIT_WIDTH <= 64
    up.frac = shift_right(shift_left(un.u, 12), 64 - POSIT_WIDTH);
#else
    up.frac = shift_left((utype)un.u, POSIT_WIDTH - 64 + 12);
#endif

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = shift_left(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}