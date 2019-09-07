#include "packing_posit.h"

class Posit {
private:
    utype mBits;
    int mNbits;
    int mEs;

public:
    Posit(utype bits, int nbits, int es);
    Posit(int nbits, int es);

    bool isZero() const;                  
    bool isNar() const;                   
    bool isNeg() const;                   
    Posit zero() const;                   
    Posit one() const;                    
    Posit nar() const;                    
    int nbits() const;                               
    int rs() const;                       // regime size 
    int es() const;                       // exponent size
    int fs() const;                       // fraction size

    Posit neg() const;                    
    Posit rec() const;                    
    Posit sqrt() const;                   

    Posit add(const Posit& p) const;     
    Posit sub(const Posit& p) const;      
    Posit mul(const Posit& p) const;      
    Posit div(const Posit& p) const;      

    bool equal(const Posit& p) const;        
    bool greater(const Posit& p) const;        
    bool greaterOrEqual(const Posit& p) const;        
    bool less(const Posit& p) const;        
    bool lessOrEqual(const Posit& p) const;        

    void set(Posit p);                    
    void set(float n);                    
    void set(double n);                   

    float getFloat() const;               
    double getDouble() const;             

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

#define CLZ(n) \
    ((n) == 0 ? 8 * sizeof(n): __builtin_clz(n))


#define shift_left(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0: (bits) << (shift))


#define shift_right(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0: (bits) >> (shift))

#define x2(n) \
    (shift_left(1, (n)))

#define floor_div(a, b) \
    ((a) / (b) - ((a) % (b) < 0))

#define MIN(a, b) \
    ((a) < (b) ? (a): (b))

#define MAX(a, b) \
    ((a) > (b) ? (a): (b))

#define LMASK(bits, size) \
    ((bits) & shift_left(posit_mask, POSIT_WIDTH - (size)))

#define HIDDEN_BIT(frac) \
    (msb_const | shift_right((frac), 1))

bool util_is_zero(utype p);
bool util_is_nar(utype p);
bool util_is_neg(utype p);

int util_rs(utype p, int nbits);
int util_es(utype p, int nbits, int es);
int util_fs(utype p, int nbits, int es);

utype util_neg(utype p, int nbits);