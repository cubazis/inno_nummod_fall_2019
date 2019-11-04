#include "blackbox.h"
#include <cstring>
#include <math.h>
#include <iostream>

#define eps 1e-9
#define N 100001
using namespace std;

void scal_mult(const double scalar, const double* vec, const int n, double* res){
    for (int i = 0; i < n; ++i){
        res[i] = scalar*vec[i];
    }
}

double dot_prod(const double* a, const double* b, const int n){
    double res = 0;
    for (int i = 0; i < n; ++i){
        res += a[i] * b[i];
    }
    return res;
}


int main(){

    // variables
    double v1[N], v2[N], x[N], b[N], r[N], r_k[N], z[N];

    blackbox_init();
    const int n = blackbox_size();

    blackbox_rhs(b);
    blackbox_mult(x, v1);

    for (int i = 0; i < n; ++i){
        r[i] = b[i] - v1[i];
    }

    memcpy(static_cast<void*>(z), static_cast<void*>(r), n * sizeof(double));

    while (sqrt(dot_prod(r, r, n)) > eps){
        blackbox_mult(z, v1);

        double alpha = dot_prod(r, r, n) / dot_prod(v1, z, n);

        scal_mult(alpha, z, n, v2);
        for (int i = 0; i < n; ++i){
            x[i] = x[i] + v2[i];
        }


        scal_mult(alpha, v1, n, v1);
        for (int i = 0; i < n; ++i){
            r_k[i] = r[i] - v1[i];
        }

        double beta = dot_prod(r_k, r_k, n) / dot_prod(r, r, n);

        scal_mult(beta, z, n, v1);
        for (int i = 0; i < n; ++i){
            z[i] = r_k[i] + v1[i];
        }

        memcpy(static_cast<void*>(r), static_cast<void*>(r_k), n * sizeof(double));
    }

    blackbox_submit(x);
    return 0;
}