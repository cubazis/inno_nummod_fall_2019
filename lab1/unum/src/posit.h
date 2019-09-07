#ifndef __POSIT_H
#define __POSIT_H

#include <stdint.h>
#include <stdbool.h>

#define CLZ(n) \
    ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define LSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define RSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define POW2(n) \
    (LSHIFT(1, (n)))

#define FLOORDIV(a, b) \
    ((a) / (b) - ((a) % (b) < 0))

#define MIN(a, b) \
    ((a) < (b) ? (a) : (b))

#define MAX(a, b) \
    ((a) > (b) ? (a) : (b))

#define LMASK(bits, size) \
    ((bits) & LSHIFT(0xFFFFFFFF, 32 - (size)))

#define HIDDEN_BIT(frac) \
    (0x80000000 | RSHIFT((frac), 1))

class posit {
private:
    uint32_t mBits;
    int mNbits;
    int mEs;

    posit(uint32_t bits, int nbits, int es);

public:
    posit();

    bool isZero() const;
    bool isNar() const;

    posit zero() const;
    posit nar() const;

    posit neg() const;

    posit add(const posit& p) const;
    posit mul(const posit& p) const;

    void set(double n);

    double getDouble() const;
    bool eq(const posit &posit) const;
};

struct unpacked_t
{
    bool neg;
    int32_t exp;
    uint32_t frac;
};

posit operator+(const posit& a, const posit& b);
posit operator-(const posit& a, const posit& b);
posit operator*(const posit& a, const posit& b);
posit operator/(const posit& a, const posit& b);

posit operator-(const posit& a);

bool operator<(const posit&a , const posit& b);
bool operator<=(const posit&a , const posit& b);
bool operator>(const posit&a , const posit& b);
bool operator>=(const posit&a , const posit& b);
bool operator==(const posit&a , const posit& b);
bool operator!=(const posit&a , const posit& b);

uint32_t pack_posit(struct unpacked_t up, int nbits, int es);
double pack_double(struct unpacked_t up);

struct unpacked_t unpack_posit(uint32_t p, int nbits, int es);
struct unpacked_t unpack_double(double f);

struct unpacked_t unpack_mul(struct unpacked_t a, struct unpacked_t b);
struct unpacked_t unpack_add(struct unpacked_t a, struct unpacked_t b);

bool is_zero(uint32_t p);
bool is_nan(uint32_t p);
bool is_neg(uint32_t p);

int get_rs(uint32_t p, int nbits);

uint32_t util_neg(uint32_t p, int nbits);

#endif
