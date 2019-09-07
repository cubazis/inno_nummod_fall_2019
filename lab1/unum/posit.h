//
// Created by arsee on 9/5/2019.
//

#ifndef UNUM32_POSIT_H
#define UNUM32_POSIT_H


#include <stdint-gcc.h>

class Posit{
    uint32_t bits;
    public:
    Posit(double d);
    Posit(float f);
    Posit(int i);
    Posit(uint32_t i);

    double toDouble();

    bool isZero() const ;
    bool isNar() const;

    Posit neg() const;
    Posit addition(const Posit &p2) const;
    Posit subtract(const Posit &p2) const ;
    Posit multiplication(const Posit &p2) const ;
    Posit division(const Posit &p2) const ;
    Posit nar() const;
    void print();
    
    bool equal(const Posit &p) const;

};

Posit operator+(const Posit& a, const Posit& b);
Posit operator-(const Posit& a, const Posit& b);
Posit operator*(const Posit& a, const Posit& b);
Posit operator/(const Posit& a, const Posit& b);
Posit operator-(const Posit& a);



#endif //UNUM32_POSIT_H
