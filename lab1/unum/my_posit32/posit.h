/**
@brief		Custom implementation of the Posit

@author		Ansat Abirov

@date		15.09.2019
*/
#pragma once

#include <cmath>
#include <string>
#ifndef POSIT32_POSIT_H
#define POSIT32_POSIT_H
#endif //POSIT32_POSIT_H


const int SZ = 32;
const int ES = 4;

class Posit {
private:
    long long get_bit(int pos) const;
    long long get_sign() const;
    long long get_regime() const;
    long long get_exp() const;
    void reverse_bit(int index);
    long long get_frac() const;
    long long get_frac_length() const;
    int* value;
public:
    Posit();
    Posit(const std::string s);
    Posit(const float f);
    Posit(const Posit &p);
    Posit(long long sign, long long regime, long long exp, long long frac, long long fs);
    ~Posit();

    static Posit INF();
    static Posit ZERO();
    void print_p();
    float to_float() const;
    bool is_zero() const;
    bool is_inf() const;

    int operator[](unsigned int pos) const;
    Posit operator*( Posit b) ;
    Posit operator+(const Posit b) const;
    Posit operator-(const Posit b) const;
    bool operator<(const Posit b) const;
};