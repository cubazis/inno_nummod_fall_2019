// posit<32, 4>

#include <iostream>
#include <cmath>

using namespace std;

const int SZ = 32;
const int ES = 4;

int* posit_inf();

int* posit_zero();

bool posit_less(int *a, int *b);

void print_p(int posit[]) {
    for (int i = 0; i < SZ; ++i) {
        cout << posit[i] << " ";
    }
    cout << endl;
}

void set_bit(int *posit, int index) {
    posit[index] = 1;
}

void reverse_bit(int *posit, int index) {
    posit[index] ^= 1;
}

/*
 * Function converts posit(32-bits array) to float
 */
float posit2float(int *posit) {
    int index = 0;

    // Sign
    int sign = (posit[index] == 0 ? 1 : -1);
    ++index;

    // Regime
    int k = 0, m = 0;
    while (++index < SZ && posit[index] == posit[1]) {
        ++m;
    }
    if (posit[1] == 0) {
        k = -m - 1;
    }
    else {
        k = m;
    }
    ++index;

    // Exponent
    long long es = 0, e = 0;
    while (index < SZ && es < ES) {
        ++es;
        e = e * 2 + posit[index];
        ++index;
    }

    // Fraction
    long long fs = 0, f = 0;
    while (index < SZ) {
        bool out = true;
        for (int j = index; j < SZ; ++j)
            if (posit[j] == 1)
                out = false;
        if (out)
            break;
        ++fs;
        f = f * 2 + posit[index];
        ++index;
    }

    long long useed = pow(2, pow(2, max_es));

    return sign * pow(useed, k) *  pow(2, e) * (1 + f * 1.0 / pow(2, fs)); // TODO: overflow problems with pow()
    //return (sign * (1ll << (16 * k + e)) * ((1ll << fs) + f) * 1.0 / (1ll << fs));
}

int* float2posit(float value) {
    int *posit = posit_zero();

    if (value == 0.0f) {
        return posit_zero();
    }/*
    if (isinf(value) || isnan(value)) {
        return posit_inf();
    }*/

    // extract sign, exponent and mantissa from float
    unsigned int* bits = (unsigned int*)&value;
    long long s = *bits >> 31;
    long long e = *bits & 0x7f800000;
    e >>= 23;
    long long mant = *bits & 0x007fffff;
    e -= 127;

    int index = 0;

    posit[index] = s;
    ++index;
    int k;
    if (e >= 0) {
        k = e / 16;
    }
    else {
        k = e / 16 - 1;
    }
    int m;
    if (k >= 0) {
        m = k + 1;
        for (int i = 0; i < m && index < SZ; ++i) {
            posit[index++] = 1;
        }
        posit[index++] = 0;
    }
    else {
        m = -k;
        for (int i = 0; i < m && index < SZ; ++i) {
            posit[index++] = 0;
        }
        posit[index++] = 1;
    }

    e = (e % 16 + 16) % 16;
    for (int i = 3; i >= 0 && index < SZ; --i) {
        posit[index++] = (((1 << i) & e) ? 1 : 0);
    }
    for (int i = 22; i >= 0 && index < SZ; --i) {
        posit[index++] = (((1 << i) & mant) ? 1 : 0);
    }
    return posit;
}


void set_array(int *posit, string s) {
    if (s[0] == '+') {
        s[0] = '0';
    }
    if (s[0] == '-') {
        s[0] = '1';
    }
    for (int i = 0; i < SZ; ++i) {
        posit[i] = (s[i] - '0');
    }
}

long long get_bit(int *p, int pos) {
    return p[pos];
}

long long get_sign(int *p) {
    return get_bit(p, 0);
}

long long get_regime(int *p) {
    for (int i = 2; i < SZ; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            int m = i - 1;
            return get_bit(p, 1) == 0 ? -m : (m - 1);
        }
    }
    return get_bit(p, 1) == 0 ? -31 : 30;
}

long long get_exp(int *p) {
    long long exp = 0;
    for (int i = 2; i < SZ; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            for (int j = i + 1; j < min(i + 5, SZ); j++) {
                exp = (exp << 1) + p[j];
            }
            break;
        }
    }
    return exp;
}

long long get_frac(int *p) {
    long long frac = 0;
    for (int i = 2; i < SZ; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            for (int j = i + 5; j < SZ; j++) {
                frac = (frac << 1) + p[j];
            }
            break;
        }
    }
    return frac;
}

long long get_frac_length(int *p) {
    long long len = 0;
    for (int i = 2; i < SZ; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            len = max((31 - (i + 5) + 1), 0);
            break;
        }
    }
    return len;
}

int* collect_posit(long long sign, long long regime, long long exp, long long frac, long long fs) {
    int *p = new int[SZ];
    for (int i = 0; i < SZ; i++) {
        p[i] = 0;
    }

    //sign
    p[0] = sign;

    //regime
    int pos = 1;
    if (regime < 0) {
        regime = abs(regime);
        for (int i = 0; i < regime; i++) {
            if (pos < SZ) {
                p[pos++] = 0;
            } else {
                break;
            }
        }
        if (pos < SZ) {
            p[pos++] = 1;
        }
    } else {
        regime++;
        for (int i = 0; i < regime; i++) {
            if (pos < SZ) {
                p[pos++] = 1;
            } else {
                break;
            }
        }
        if (pos < SZ) {
            p[pos++] = 0;
        }
    }

    //exponent
    for (int i = 3; i >= 0; i--) {
        if (pos < SZ) {
            p[pos++] = (exp >> i) % 2;
        } else {
            break;
        }
    }

    //fraction
    for (int i = fs; i >= 0; i--) {
        if (pos < SZ) {
            p[pos++] = (frac >> i) % 2;
        } else {
            break;
        }
    }

    return p;
}

int* posit_zero() {
    int *p = new int[SZ];
    for (int i = 0; i < SZ; i++) {
        p[i] = 0;
    }
    return p;
}

int* posit_inf() {
    int *p = new int[SZ];
    p[0] = 1;
    for (int i = 1; i < SZ; i++) {
        p[i] = 0;
    }
    return p;
}

bool is_zero(int *p) {
    bool all_zeros = true;
    for (int i = 0; i < SZ; i++) {
        all_zeros &= p[i] == 0;
    }
    return all_zeros;
}

bool is_inf(int *p) {
    bool inf = p[0] == 1;
    for (int i = 1; i < SZ; i++) {
        inf &= p[i] == 0;
    }
    return inf;
}

int* multiply(int *a, int *b) {
    if (is_zero(a) || is_zero(b)) {
        return posit_zero();
    }

    if (is_inf(a) || is_inf(b)) {
        return posit_inf();
    }

    int sign = get_sign(a) ^ get_sign(b);

    long long fs1 = get_frac_length(a);
    long long fs2 = get_frac_length(b);

    long long t1 = (1LL << fs1) + get_frac(a);
    long long t2 = (1LL << fs2) + get_frac(b);

    long long exp = get_exp(a) + get_exp(b) + ((t1 * t2) >> (fs1 + fs2 + 1));

    long long frac = ((t1 * t2) & ((1LL << (fs1 + fs2)) - 1)) << 1;

    long long regime = get_regime(a) + get_regime(b) + (exp / (1LL << ES));
    exp %= (1LL << ES);

    return collect_posit(sign, regime, exp, frac, fs1 + fs2);
}

int* sum(int *a, int *b) {
    if (is_zero(a)) {
        return b;
    }

    if (is_zero(b)) {
        return a;
    }

    if (is_inf(a) || is_inf(b)) {
        return posit_inf();
    }

    if (posit_less(a, b)) {
        return sum(b, a);
    }

    int sign = get_sign(a) ^ get_sign(b);

    // subtraction
    if (sign) {
        return subtraction(a, b);
    }

    long long fs1 = get_frac_length(a);
    long long fs2 = get_frac_length(b);
    return posit_zero();
}

// return true if (a < b)
bool posit_less(int *a, int *b) {
    int sign_a = get_sign(a);
    int sign_b = get_sign(b);
    if (!sign_b && sign_a) {
        return true;
    }
    if (sign_b && !sign_a) {
        return false;
    }

    if (sign_a && sign_b) {
        reverse_bit(a, 0);
        reverse_bit(b, 0);

    }
}

int main() {
    int *posit = posit_zero();

    print_p(posit);
    printf(" %.8f\n", posit2float(posit));
    set_array(posit, "-1000000000000000000000000000000");

    print_p(posit);
    printf(" %.30f\n", posit2float(posit));


    return 0;
}