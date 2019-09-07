//
// Created by ansat on 19/09/07.
//
#include "posit.h"
#include <cmath>
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>

Posit::Posit() {
    value = new int[SZ];
    for (int i = 0; i < SZ; i++) {
        value[i] = 0;
    }
}

Posit::~Posit() {
    delete[] value;
}

Posit::Posit(const Posit &p) {
    Posit();
    this->value = new int[SZ];
    for (int i = 0; i < SZ; i++) {
        this->value[i] = p.value[i];
    }
}

int Posit::operator[](unsigned int pos) const {
    return this->value[pos];
}

void Posit::print_p() {
    for (int i = 0; i < SZ; ++i) {
        printf("%d ", value[i]);
    }
    printf("\n");
}

void Posit::reverse_bit(int index) {
    value[index] ^= 1;
}

/*
 * Function converts posit(32-bits array) to float
 */
float Posit::to_float() const {
    int index = 0;

    // Sign
    int sign = (value[index] == 0 ? 1 : -1);
    ++index;

    // Regime
    int k = 0, m = 0;
    while (++index < SZ && value[index] == value[1]) {
        ++m;
    }
    if (value[1] == 0) {
        k = -m - 1;
    } else {
        k = m;
    }
    ++index;

    // Exponent
    long long es = 0, e = 0;
    while (index < SZ && es < ES) {
        ++es;
        e = e * 2 + value[index];
        ++index;
    }

    // Fraction
    long long fs = 0, f = 0;
    while (index < SZ) {
        bool out = true;
        for (int j = index; j < SZ; ++j)
            if (value[j] == 1)
                out = false;
        if (out)
            break;
        ++fs;
        f = f * 2 + value[index];
        ++index;
    }

    long long useed = pow(2, pow(2, ES));

    return sign * pow(useed, k) * pow(2, e) * (1 + f * 1.0 / pow(2, fs)); // TODO: overflow problems with pow()
}

Posit::Posit(const float f) {
    Posit();
    this->value = new int[SZ];

    if (f == 0.0f) {
        for (int i = 0; i < SZ; ++i) {
            this->value[i] = 0;
        }
        return;
    }

    if (std::isinf(f) || std::isnan(f)) {
        for (int i = 0; i < SZ; ++i) {
            this->value[i] = 0;
        }
        this->value[0] = 1;
        return;
    }

    // extract sign, exponent and mantissa from float
    unsigned int *bits = (unsigned int *) &f;
    long long s = *bits >> 31;
    long long e = *bits & 0x7f800000;
    e >>= 23;
    long long mant = *bits & 0x007fffff;
    e -= 127;

    int index = 0;

    value[index] = s;
    ++index;
    int k;
    if (e >= 0) {
        k = e / 16;
    } else {
        k = e / 16 - 1;
    }
    int m;
    if (k >= 0) {
        m = k + 1;
        for (int i = 0; i < m && index < SZ; ++i) {
            value[index++] = 1;
        }
        value[index++] = 0;
    } else {
        m = -k;
        for (int i = 0; i < m && index < SZ; ++i) {
            value[index++] = 0;
        }
        value[index++] = 1;
    }

    e = (e % 16 + 16) % 16;
    for (int i = 3; i >= 0 && index < SZ; --i) {
        value[index++] = (((1ll << i) & e) ? 1 : 0);
    }
    for (int i = 22; i >= 0 && index < SZ; --i) {
        value[index++] = (((1ll << i) & mant) ? 1 : 0);
    }
}


Posit::Posit(std::string s) {
    Posit();
    if (s[0] == '+') {
        s[0] = '0';
    }
    if (s[0] == '-') {
        s[0] = '1';
    }
    for (int i = 0; i < SZ; ++i) {
        value[i] = (s[i] - '0');
    }
}

long long Posit::get_bit(int pos) const {
    return value[pos];
}

long long Posit::get_sign() const {
    return get_bit(0);
}

long long Posit::get_regime() const {
    for (int i = 2; i < SZ; i++) {
        if (get_bit(i) != get_bit(1)) {
            int m = i - 1;
            return get_bit(1) == 0 ? -m : (m - 1);
        }
    }
    return get_bit(1) == 0 ? -31 : 30;
}

long long Posit::get_exp() const {
    long long exp = 0;
    for (int i = 2; i < SZ; i++) {
        if (get_bit(i) != get_bit(1)) {
            for (int j = i + 1; j < std::min(i + 5, SZ); j++) {
                exp = (exp << 1) + value[j];
            }
            break;
        }
    }
    return exp;
}

long long Posit::get_frac() const {
    long long frac = 0;
    for (int i = 2; i < SZ; i++) {
        if (get_bit(i) != get_bit(1)) {
            for (int j = i + 5; j < SZ; j++) {
                frac = (frac << 1) + value[j];
            }
            break;
        }
    }
    return frac;
}

long long Posit::get_frac_length() const {
    long long len = 0;
    for (int i = 2; i < SZ; i++) {
        if (get_bit(i) != get_bit(1)) {
            len = std::max((31 - (i + 5) + 1), 0);
            break;
        }
    }
    return len;
}

Posit::Posit(long long sign, long long regime, long long exp, long long frac, long long fs) {
    Posit();
    value = new int[SZ];
    for (int i = 0; i < SZ; i++) {
        value[i] = 0;
    }

    //sign
    value[0] = sign;

    //regime
    int pos = 1;
    if (regime < 0) {
        regime = std::abs(regime);
        for (int i = 0; i < regime; i++) {
            if (pos < SZ) {
                value[pos++] = 0;
            } else {
                break;
            }
        }
        if (pos < SZ) {
            value[pos++] = 1;
        }
    } else {
        regime++;
        for (int i = 0; i < regime; i++) {
            if (pos < SZ) {
                value[pos++] = 1;
            } else {
                break;
            }
        }
        if (pos < SZ) {
            value[pos++] = 0;
        }
    }

    //exponent
    for (int i = 3; i >= 0; i--) {
        if (pos < SZ) {
            value[pos++] = (exp >> i) % 2;
        } else {
            break;
        }
    }

    //fraction
    for (int i = fs; i >= 0; i--) {
        if (pos < SZ) {
            value[pos++] = (frac >> i) % 2;
        } else {
            break;
        }
    }
}

Posit Posit::ZERO() {
    return Posit();
}

Posit Posit::INF() {
    Posit p = Posit();
    p.value[0] = 1;
    for (int i = 1; i < SZ; i++) {
        p.value[i] = 0;
    }
    return p;
}

Posit Posit::operator*(Posit b) {

    if (this->is_zero() || b.is_zero()) {
        return ZERO();
    }

    if (this->is_inf() || b.is_inf()) {
        return INF();
    }

    int sign = this->get_sign() ^b.get_sign();

    long long fs1 = this->get_frac_length();
    long long fs2 = b.get_frac_length();

    long long t1 = (1LL << fs1) + this->get_frac();
    long long t2 = (1LL << fs2) + b.get_frac();

    long long exp = this->get_exp() + b.get_exp() + ((t1 * t2) >> (fs1 + fs2 + 1));

    long long frac = ((t1 * t2) & ((1LL << (fs1 + fs2)) - 1)) << 1;

    long long regime = this->get_regime() + b.get_regime() + (exp / (1LL << ES));
    exp %= (1LL << ES);

    return Posit(sign, regime, exp, frac, fs1 + fs2);
}

bool Posit::is_zero() const {
    bool all_zeros = true;
    for (int i = 0; i < SZ; i++) {
        all_zeros &= value[i] == 0;
    }
    return all_zeros;
}

bool Posit::is_inf() const {
    bool inf = value[0] == 1;
    for (int i = 1; i < SZ; i++) {
        inf &= value[i] == 0;
    }
    return inf;
}

Posit Posit::operator-(const Posit b) const {
    return Posit();
}

bool Posit::operator<(const Posit b) const {
    int sign_a = this->get_sign();
    int sign_b = b.get_sign();
    if (!sign_b && sign_a) {
        return true;
    }
    if (sign_b && !sign_a) {
        return false;
    }

    // Check negative posits
    if (sign_a && sign_b) {
        Posit copy_a = Posit(*this),
                copy_b = Posit(b);
        copy_a.reverse_bit(0);
        copy_b.reverse_bit(0);
        bool ret = (copy_a < copy_b);
        return !ret;
    }

    if (this->get_regime() < b.get_regime()) {
        return true;
    }
    if (this->get_regime() > b.get_regime()) {
        return false;
    }

    if (this->get_exp() < b.get_exp()) {
        return true;
    }
    if (this->get_exp() > b.get_exp()) {
        return false;
    }
    if (this->get_frac() < b.get_frac()) {
        return true;
    }
    return false;

}

Posit Posit::operator+(const Posit b) const {
    if (this->is_zero()) {
        return b;
    }

    if (b.is_zero()) {
        return *this;
    }

    if (this->is_inf() || b.is_inf()) {
        return INF();
    }

    if (*this < b) {
        return b + *this;
    }

    int sign = this->get_sign() ^ b.get_sign();

    // subtraction
    if (sign) {
        return *this - b;
    }

    long long fs1 = this->get_frac_length();
    long long fs2 = b.get_frac_length();

    long long f1 = this->get_frac() + (1LL << fs1);
    long long f2 = b.get_frac() + (1LL << fs2);

    long long delta = ((this->get_regime() - b.get_regime()) << ES) + (this->get_exp() - b.get_exp());

    std::vector<int> f;
    for (int i = fs1; i >= 0; i--) {
        f.push_back((f1 >> i) % 2);
    }
    for (int i = 0; i < delta; i++) {
        f.push_back(0);
    }

    for (int i = 0; i < 64; i++) {
        f[f.size() - i - 1] += (f2 >> i) % 2;
    }

    for (int i = f.size() - 1; i > 0; i--) {
        f[i - 1] += f[i] / 2;
        f[i] %= 2;
    }

    if (f[0] > 1) {
        std::vector<int> tmp;
        tmp.push_back(1);
        f[0] %= 2;
        for (int i = 0; i < f.size(); i++) {
            tmp.push_back(f[i]);
        }
        f = tmp;
    }

    long long exp = this->get_exp() + (f.size() > fs1 + 1 + delta ? 1LL : 0LL);
    long long regime = this->get_regime() + (exp / (1LL << ES));
    exp %= (1LL << ES);

    f.erase(f.begin());

    long long frac = 0;
    for (int i = 0; i < SZ - ES - 3; i++) {
        frac = (frac << 1) + (i < f.size() ? f[i] : 0);
    }

    return Posit(sign, regime, exp, frac, SZ - ES - 4);
}