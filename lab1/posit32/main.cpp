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

unsigned int get_bit(int *p, int pos) {
    return p[pos];
}

int get_sign(int *p) {
    return get_bit(p, 0);
}

int get_regime(int *p) {
    int cnt = 1;
    for (int i = 2; i < 32; i++) {
        
    }
}

int* multiply(int *a, int *b) {
    int *result = new int[32];

}

int main() {
    /*
    std::cout << "Hello, World!" << std::endl;
    float tmp = 100;
    cout << bitset<sizeof tmp*8>(*(long unsigned int*)(&tmp))<< endl;
    */

    int posit[32] = {0};

    print_p(posit);

    set_bit(posit, 2);

    print_p(posit);

    set_array(posit, "+0111011011100000010000011000101");

    print_p(posit);

    float tmp;
    posit2float(posit, tmp);

    cout << tmp << endl;

    float2posit(tmp, posit);

    print_p(posit);

    return 0;
}
