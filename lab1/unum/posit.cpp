//
// Created by arsee on 9/5/2019.
//

#include <cmath>
#include <iostream>
#include <bitset>

#include "posit.h"
extern "C" {
#include "posit_constants.h"
#include "converter.h"
#include "util.h"
#include "operations.h"
}
using namespace std;


bool Posit::isZero() const {
    return util_is_zero(bits);
}

bool Posit::isNar() const {
    return util_is_nar(bits);
}

Posit Posit::nar() const
{
    return Posit(POSIT_NAR);
}

Posit Posit::addition(const Posit &p2) const {
    if (isNar() || p2.isNar()) {
        return nar();
    } else if (isZero()) {
        return p2;
    } else if (p2.isZero()) {
        return *this;
    } else if (neg().equal(p2)) {
        return Posit(POSIT_ZERO);
    }

    return Posit(summ(bits, p2.bits));
}

Posit Posit::neg() const{
     if (isNar()) {
        return nar();
    }

    return Posit(util_neg(bits, 32));
}

Posit Posit::subtract(const Posit &p2) const{
    if (isNar() || p2.isNar()) {
        return nar();
    } else if (isZero()) {
        return p2.neg();
    } else if (p2.isZero()) {
        return *this;
    } else if (equal(p2)) {
        return Posit(POSIT_ZERO);
    }
    return dif(bits,p2.bits);
}

Posit Posit::multiplication(const Posit &p2) const {
    if (isNar() || p2.isNar()) {
        return nar();
    } else if (isZero() || p2.isZero()) {
        return Posit(POSIT_ZERO);
    }

    return Posit(mult(bits,p2.bits));
}

Posit Posit::division(const Posit &p2) const {
    if (isNar() || p2.isNar() || p2.isZero()) {
        return nar();
    } else if (isZero()) {
        return Posit(POSIT_ZERO);
    }
    return Posit(divis(bits,p2.bits));
}

bool Posit::equal(const Posit &p) const {
    return p.bits==bits;
}

double Posit::toDouble() {
    return fromPosit(bits);
}
Posit::Posit(uint32_t b) {bits = b;}


Posit::Posit(double d) {
    switch (fpclassify(d)) {
        case FP_INFINITE:
        case FP_NAN:
            bits = POSIT_NAR;
            break;
        case FP_ZERO:
            bits = POSIT_ZERO;
            break;
        default:
            bits =double_to_posit(d);
            break;
    }
}
Posit::Posit(float f){
    double d= static_cast<double >(f);
    bits = Posit(d).bits;
}

Posit::Posit(int i) {
    double d = static_cast<double >(i);
    bits = Posit(d).bits;
}

Posit operator+(const Posit& a, const Posit& b)
{
    return a.addition(b);
}

Posit operator-(const Posit& a, const Posit& b)
{
    return a.subtract(b);
}

Posit operator*(const Posit& a, const Posit& b)
{
    return a.multiplication(b);
}

Posit operator/(const Posit& a, const Posit& b)
{
    return a.division(b);
}

Posit operator-(const Posit& a)
{
    return a.neg();
}

void Posit::print() {
    cout<<std::bitset<32>(bits);
}







