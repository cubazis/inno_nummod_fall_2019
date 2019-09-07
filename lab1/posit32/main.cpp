// posit<32, 4>

#include <iostream>
#include <bitset>
#include <cmath>

using namespace std;

void print_p(int posit[]) {
    for (int i = 0; i < 32; ++i) {
        cout << posit[i] << " ";
    }
    cout << endl;
}

void set_bit(int *posit, int index) {
    posit[index] = 1;
}
/*
 * Function converts posit(32-bits array) to float
 */
void posit2float(int *posit, float &res) {
    int index = 0;

    // Sign
    int sign = (posit[index] == 0 ? 1 : -1);
    ++index;

    // Regime
    int k = 0, m = 0;
    while (++index < 32 && posit[index] == posit[1]) {
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
    int max_es = 4, es = 0, e = 0;
    while (index < 32 && es < max_es) {
        ++es;
        e = e * 2 + posit[index];
        ++index;
    }

    // Fraction
    int fs = 0, f = 0;
    while (index < 32) {
        bool out = true;
        for (int j = index; j < 32; ++j)
            if (posit[j] == 1)
                out = false;
        if (out)
            break;
        ++fs;
        f = f * 2 + posit[index];
        ++index;
    }
/*
 *  DEBUG:
    cout << "sign = " << sign << endl;
    cout << "k = " << k << endl;
    cout << "e = " << e << endl;
    cout << "fs = " << fs << endl;
*/
    int useed = pow(2, pow(2, max_es)); // TODO: max_es or es

    res = sign * pow(useed, k) *  pow(2, e) * (1 + f * 1.0 / pow(2, fs)); // TODO: overflow problems with pow()
//    return (1.0 * sign * (1 << (16 * k + e)) * ((1 << fs) + f) / (1 << fs));
}

void float2posit(float value, int *posit) {
    // extract sign, exponent and mantissa from float
    unsigned int* bits = (unsigned int*)&value;
    int s = *bits >> 31;
    int e = *bits & 0x7f800000;
    e >>= 23;
    int mant = *bits & 0x007fffff;
    e -= 127;
    //cout << s << " " << e << " " << mant << endl;
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
        for (int i = 0; i < m && index < 32; ++i) {
            posit[index++] = 1;
        }
        posit[index++] = 0;
    }
    else {
        m = -k;
        for (int i = 0; i < m && index < 32; ++i) {
            posit[index++] = 0;
        }
        posit[index++] = 1;
    }

    e = (e % 16 + 16) % 16;
    for (int i = 3; i >= 0 && index < 32; --i) {
        posit[index++] = (((1 << i) & e) ? 1 : 0);
    }
    for (int i = 22; i >= 0 && index < 32; --i) {
        posit[index++] = (((1 << i) & mant) ? 1 : 0);
    }
}


void set_array(int *posit, string s) {
    if (s[0] == '+')
        s[0] = '0';
    if (s[0] == '-')
        s[0] = '1';
    for (int i = 0; i < 32; ++i) {
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
    for (int i = 2; i < 32; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            int m = i - 1;
            return get_bit(p, 1) == 0 ? -m : (m - 1);
        }
    }
    return get_bit(p, 1) == 0 ? -31 : 30;
}

long long get_exp(int *p) {
    long long exp = 0;
    for (int i = 2; i < 32; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            for (int j = i + 1; j < min(i + 5, 32); j++) {
                exp = (exp << 1) + p[j];
            }
            break;
        }
    }
    return exp;
}

long long get_frac(int *p) {
    long long frac = 0;
    for (int i = 2; i < 32; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            for (int j = i + 5; j < 32; j++) {
                frac = (frac << 1) + p[j];
            }
            break;
        }
    }
    return frac;
}

long long get_frac_length(int *p) {
    long long len = 0;
    for (int i = 2; i < 32; i++) {
        if (get_bit(p, i) != get_bit(p, 1)) {
            len = max((31 - (i + 5) + 1), 0);
            break;
        }
    }
    return len;
}

int* collect_posit(long long sign, long long regime, long long exp, long long frac, long long fs) {
    int *p = new int[32];
    for (int i = 0; i < 32; i++) {
        p[i] = 0;
    }

    //sign
    p[0] = sign;

    //regime
    int pos = 1;
    if (regime < 0) {
        regime = abs(regime);
        for (int i = 0; i < regime; i++) {
            if (pos < 32) {
                p[pos++] = 0;
            } else {
                break;
            }
        }
        if (pos < 32) {
            p[pos++] = 1;
        }
    } else {
        regime++;
        for (int i = 0; i < regime; i++) {
            if (pos < 32) {
                p[pos++] = 1;
            } else {
                break;
            }
        }
        if (pos < 32) {
            p[pos++] = 0;
        }
    }

    //exponent
    for (int i = 3; i >= 0; i--) {
        if (pos < 32) {
            p[pos++] = (exp >> i) % 2;
        } else {
            break;
        }
    }

    //fraction
    for (int i = fs; i >= 0; i--) {
        if (pos < 32) {
            p[pos++] = (frac >> i) % 2;
        } else {
            break;
        }
    }

    return p;
}

int* posit_zero() {
    int *p = new int[32];
    for (int i = 0; i < 32; i++) {
        p[i] = 0;
    }
    return p;
}

int* posit_inf() {
    int *p = new int[32];
    p[0] = 1;
    for (int i = 1; i < 32; i++) {
        p[i] = 0;
    }
    return p;
}

bool is_zero(int *p) {
    bool all_zeros = true;
    for (int i = 0; i < 32; i++) {
        all_zeros &= p[i] == 0;
    }
    return all_zeros;
}

bool is_inf(int *p) {
    bool inf = p[0] == 1;
    for (int i = 1; i < 32; i++) {
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

    long long regime = get_regime(a) + get_regime(b) + (exp / (1LL << 4));
    exp %= (1LL << 4);

    return collect_posit(sign, regime, exp, frac, fs1 + fs2);
}

int main() {
    /*
    std::cout << "Hello, World!" << std::endl;
    float tmp = 100;
    cout << bitset<sizeof tmp*8>(*(long unsigned int*)(&tmp))<< endl;
    */

    int posit[32] = {0};

    print_p(posit);

    // set_bit(posit, 2);
    //
    // print_p(posit);

    set_array(posit, "+0111011011100000010000011000101");

    print_p(posit);

    cout << get_sign(posit) << endl;
    cout << get_regime(posit) << endl;
    cout << get_exp(posit) << endl;
    cout << get_frac(posit) << endl;

    float a = 4.23;
    float b = -4.0;

    int pa[32] = {0};
    float2posit(a, pa);

    cout << "a = " << a << endl;
    print_p(pa);

    int pb[32] = {0};
    float2posit(b, pb);

    cout << "b = " << b << endl;
    print_p(pb);

    int* pc = multiply(pa, pb);

    float c;
    posit2float(pc, c);

    cout << c << endl;
    print_p(pc);

    // float tmp;
    // posit2float(posit, tmp);
    //
    // cout << tmp << endl;
    //
    // float2posit(tmp, posit);
    //
    // print_p(posit);

    return 0;
}
