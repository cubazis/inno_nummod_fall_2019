#ifndef __POSIT_H
#define __POSIT_H
#include <stdint.h>
#include <stdbool.h>

#define POSIT_WIDTH     32
#define POSIT_ZERO      ((uint32_t)0x00000000)
#define POSIT_ONE       ((uint32_t)0x40000000)
#define POSIT_NAR       ((uint32_t)0x80000000)
#define POSIT_MSB       ((uint32_t)0x80000000)
#define POSIT_MASK      ((uint32_t)0xFFFFFFFF)


class Posit {
private:
    uint32_t mBits;
    int mNbits;
    int mEs;

public:
    Posit(uint64_t bits, int nbits, int es);
    Posit(int nbits, int es);

    bool isZero() const;                  // check for 0
    bool isNar() const;                   // check for NaR
    bool isNeg() const;                   // check for negative

    int nbits() const;                    // size in bits
    int ss() const;                       // sign size in bits
    int rs() const;                       // regime size in bits
    int es() const;                       // exponent size in bits
    int fs() const;                       // fraction size in bits

    Posit zero() const;                   // 0
    Posit one() const;                    // 1
    Posit nar() const;                    // NaR

    Posit neg() const;                    // -x
    Posit rec() const;                    // 1 / x
    Posit sqrt() const;                   // sqrt(x)

    Posit add(const Posit& p) const;      // x + p
    Posit sub(const Posit& p) const;      // x - p
    Posit mul(const Posit& p) const;      // x * p
    Posit div(const Posit& p) const;      // x / p

    bool eq(const Posit& p) const;        // x == p
    bool gt(const Posit& p) const;        // x > p
    bool ge(const Posit& p) const;        // x >= p
    bool lt(const Posit& p) const;        // x < p
    bool le(const Posit& p) const;        // x <= p

    void set(Posit p);                    // x = p
    void set(float n);                    // x = n
    void set(double n);                   // x = n

    float getFloat() const;               // n = x
    double getDouble() const;             // n = x

    // debug
    void setBits(uint64_t bits);
    uint64_t getBits();
    void print();
};

class Posit8 : public Posit
{
public:
    Posit8();
    Posit8(Posit v);
    Posit8(float v);
    Posit8(double v);
};

class Posit16 : public Posit
{
public:
    Posit16();
    Posit16(Posit v);
    Posit16(float v);
    Posit16(double v);
};

class Posit32 : public Posit
{
public:
    Posit32();
    Posit32(Posit v);
    Posit32(float v);
    Posit32(double v);
};

Posit operator+(const Posit& a, const Posit& b);
Posit operator-(const Posit& a, const Posit& b);
Posit operator*(const Posit& a, const Posit& b);
Posit operator/(const Posit& a, const Posit& b);

Posit operator-(const Posit& a);

bool operator<(const Posit&a , const Posit& b);
bool operator<=(const Posit&a , const Posit& b);
bool operator>(const Posit&a , const Posit& b);
bool operator>=(const Posit&a , const Posit& b);
bool operator==(const Posit&a , const Posit& b);
bool operator!=(const Posit&a , const Posit& b);

#endif
