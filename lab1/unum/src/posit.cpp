#include "posit.h"

#include <cmath>

using namespace std;

posit::posit(uint32_t bits, int nbits, int es) :
    mBits(bits),
    mNbits(nbits),
    mEs(es)
{}

posit::posit() : posit(0x00000000, 32, 2) {}

bool posit::isZero() const
{
    return is_zero(mBits);
}

bool posit::isNar() const
{
    return is_nan(mBits);
}

posit posit::zero() const
{
    return posit(0x00000000, mNbits, mEs);
}

posit posit::nar() const
{
    return posit(0x80000000, mNbits, mEs);
}

posit posit::neg() const
{
    if (isNar()) {
        return nar();
    }

    return posit(util_neg(mBits, mNbits), mNbits, mEs);
}

posit posit::add(const posit& p) const
{
    if (isNar() || p.isNar()) {
        return nar();
    } else if (isZero()) {
        return p;
    } else if (p.isZero()) {
        return *this;
    } else if (neg().eq(p)) {
        return zero();
    }

    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t bup = unpack_posit(p.mBits, p.mNbits, p.mEs);
    unpacked_t up = unpack_add(aup, bup);

    return posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

posit posit::mul(const posit& p) const
{
    if (isNar() || p.isNar()) {
        return nar();
    } else if (isZero() || p.isZero()) {
        return zero();
    }

    unpacked_t aup = unpack_posit(mBits, mNbits, mEs);
    unpacked_t bup = unpack_posit(p.mBits, p.mNbits, p.mEs);
    unpacked_t up = unpack_mul(aup, bup);

    return posit(pack_posit(up, mNbits, mEs), mNbits, mEs);
}

void posit::set(double n)
{
    switch (fpclassify(n)) {
    case FP_INFINITE:
    case FP_NAN:
        mBits = 0x80000000;
        break;
    case FP_ZERO:
        mBits = 0x00000000;
        break;
    default:
        mBits = pack_posit(unpack_double(n), mNbits, mEs);
        break;
    }
}

double posit::getDouble() const
{
    if (isZero()) {
        return 0.0;
    } else if (isNar()) {
        return 0.0 / 0.0;
    }

    return pack_double(unpack_posit(mBits, mNbits, mEs));
}

bool posit::eq(const posit &a) const {
    return this->mBits == a.mBits;
}

posit operator+(const posit& a, const posit& b)
{
    return a.add(b);
}

posit operator*(const posit& a, const posit& b)
{
    return a.mul(b);
}

posit operator-(const posit& a)
{
    return a.neg();
}

uint32_t pack_posit(struct unpacked_t up, int nbits, int es)
{
    uint32_t p;
    uint32_t regbits;
    uint32_t expbits;

    int maxexp = POW2(es) * (nbits - 2);
    if (up.exp < -maxexp) {
        up.exp = -maxexp;
    } else if (up.exp > maxexp) {
        up.exp = maxexp;
    }

    int reg = FLOORDIV(up.exp, POW2(es));
    int ss = 1;
    int rs = MAX(-reg + 1, reg + 2);

    // FIXME: round exponent up if needed
    if (ss + rs + es >= nbits && up.frac >= 0x80000000) {
        up.exp++;
        reg = FLOORDIV(up.exp, POW2(es));
        rs = MAX(-reg + 1, reg + 2);
    }

    uint32_t exp = up.exp - POW2(es) * reg;

    if (reg < 0) {
        regbits = RSHIFT(0x80000000, -reg);
    } else {
        regbits = LMASK(0xFFFFFFFF, reg + 1);
    }
    expbits = LMASK(LSHIFT(exp, 32 - es), es);

    p = up.frac;
    p = expbits | RSHIFT(p, es);
    p = regbits | RSHIFT(p, rs);
    p = RSHIFT(p, ss);

    if (up.neg) {
        return util_neg(p, nbits);
    } else {
        return LMASK(p, nbits);
    }
}

double pack_double(struct unpacked_t up)
{
    int fexp = up.exp + 1023;

    uint64_t fexpbits;
    uint64_t ffracbits;

    if (fexp > 2046) {
        fexpbits = LSHIFT((uint64_t)2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = LSHIFT((uint64_t)(0x80000000 | RSHIFT(up.frac, 1)), 64 - 32);
        ffracbits = RSHIFT(ffracbits, -fexp);
    } else {
        fexpbits = LSHIFT((uint64_t)(fexp & 0x7FF), 53);
        ffracbits = LSHIFT((uint64_t)up.frac, 64 - 32);
    }

    union {
        double f;
        uint64_t u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | RSHIFT(un.u, 11);
    un.u = LSHIFT((uint64_t)up.neg, 63) | RSHIFT(un.u, 1);

    if (LSHIFT(un.u, 1) == 0) {
        un.u++;
    }

    return un.f;
}

struct unpacked_t unpack_posit(uint32_t p, int nbits, int es)
{
    struct unpacked_t up;

    bool neg = is_neg(p);
    if (neg) {
        p = util_neg(p, nbits);
    }

    int ss = 1;
    int rs = get_rs(p, nbits);

    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss) | 1);

    int reg = (lz == 0 ? lo - 1 : -lz);
    uint32_t exp = RSHIFT(LSHIFT(p, ss + rs), 32 - es);

    up.neg = neg;
    up.exp = POW2(es) * reg + exp;
    up.frac = LSHIFT(p, ss + rs + es);

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

    up.neg = RSHIFT(un.u, 63);
    up.exp = (RSHIFT(un.u, 52) & 0x7FF) - bias;
    up.frac = RSHIFT(LSHIFT(un.u, 12), 64 - 32);

    if (up.exp == -bias) {
        up.exp -= CLZ(up.frac);
        up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
    }

    return up;
}

static struct unpacked_t add(struct unpacked_t a, struct unpacked_t b, bool neg)
{
    struct unpacked_t r;

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
    if (RSHIFT(frac, 32) != 0) {
        r.exp++;
        frac = RSHIFT(frac, 1);
    }

    r.neg = neg;
    r.frac = LSHIFT(frac, 1);

    return r;
}

static struct unpacked_t sub(struct unpacked_t a, struct unpacked_t b, bool neg)
{
    struct unpacked_t r;

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

struct unpacked_t unpack_mul(struct unpacked_t a, struct unpacked_t b)
{
    struct unpacked_t r;

    uint64_t afrac = HIDDEN_BIT(a.frac);
    uint64_t bfrac = HIDDEN_BIT(b.frac);
    uint32_t frac = RSHIFT(afrac * bfrac, 32);
    int32_t exp = a.exp + b.exp + 1;

    if ((frac & 0x80000000) == 0) {
        exp--;
        frac = LSHIFT(frac, 1);
    }

    r.neg = a.neg ^ b.neg;
    r.exp = exp;
    r.frac = LSHIFT(frac, 1);

    return r;
}

struct unpacked_t unpack_add(struct unpacked_t a, struct unpacked_t b)
{
    if (a.neg == b.neg) {
        return add(a, b, a.neg);
    } else {
        return sub(a, b, a.neg);
    }
}

bool is_zero(uint32_t p)
{
    return p == 0x00000000;
}

bool is_nan(uint32_t p)
{
    return p == 0x80000000;
}

bool is_neg(uint32_t p)
{
    return (int32_t)p < 0 && !is_nan(p);
}

int get_rs(uint32_t p, int nbits)
{
    int ss = 1;
    int lz = CLZ(LSHIFT(p, ss));
    int lo = CLZ(LSHIFT(~p, ss));
    int rs = MAX(lz, lo) + 1;

    return MIN(rs, nbits - ss);
}

uint32_t util_neg(uint32_t p, int nbits)
{
    return LMASK(-LMASK(p, nbits), nbits);
}
