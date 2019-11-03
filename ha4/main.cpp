/**
 * This code implements conjugate gradient method for SLAE solving
 */
#include "blackbox.h"
#include <cstring>
#include <math.h>
#include <iostream>

using namespace std;

/**
 * This is a simple dot-product function for two vectors
 * @param a - vector 1
 * @param b - vector 2
 * @param n - size of BOTH vectors (must be the same)
 * @return double res - dot product of a and b
 */
double dot_prod(const double* a, const double* b, const int n){
    double res = 0;
    for (int i = 0; i < n; ++i){
        res += a[i] * b[i];
    }
    return res;
}

/**
 * This is a scalar multiplication for a vector
 * @param scalar - double scalar
 * @param vec - vector
 * @param n - size of vector
 * @param res  - result of scalar multiplication, vector of size n
 */
void scal_mult(const double scalar, const double* vec, const int n, double* res){
    for (int i = 0; i < n; ++i){
        res[i] = scalar*vec[i];
    }
}

// temporary variables
double var1[10001];
double var2[10001];

double x[10001]; // solution
double b[10001]; // right-hand side of the SLAE
double r[10001]; // residual at k-1 step
double r_k[10001]; // residual at k step
double z[10001]; // basis vector

int main(){
    double eps = 1e-9;

    blackbox_init();
    int n = blackbox_size();
    blackbox_rhs(b);

    // Using algorithm from wikipedia ("Метод сопряжённых градиентов (для решения СЛАУ)")
    // Подготовка перед итерационным процессом
    // Выберем начальное приближение - x_0
    blackbox_mult(x, var1); // var1 = A*x_0
    for (int i = 0; i < n; ++i){ // r_0 = b - A*x_0
        r[i] = b[i] - var1[i];
    }
    memcpy(static_cast<void*>(z), static_cast<void*>(r), n * sizeof(double)); // z_0 = r_0

    while (sqrt(dot_prod(r, r, n)) > eps){
        // calculating alpha_k
        blackbox_mult(z, var1); // var1 = A*z_{k-1}
        double alpha = dot_prod(r, r, n) / dot_prod(var1, z, n);

        // calculating x_k
        scal_mult(alpha, z, n, var2); // var2 = alpha_k * z_{k-1}
        for (int i = 0; i < n; ++i){
            x[i] = x[i] + var2[i];
        }

        // calculating r_k
        scal_mult(alpha, var1, n, var1); // var1 = alpha_k * A * z_{k-1}
        for (int i = 0; i < n; ++i){
            r_k[i] = r[i] - var1[i];
        }

        // calculating beta_k
        double beta = dot_prod(r_k, r_k, n) / dot_prod(r, r, n);

        // calculating z_k
        scal_mult(beta, z, n, var1); // var1 = beta_k * z_{k-1}
        for (int i = 0; i < n; ++i){
            z[i] = r_k[i] + var1[i];
        }

        // copying r_k to r_{k-1}
        memcpy(static_cast<void*>(r), static_cast<void*>(r_k), n * sizeof(double));

    }
    blackbox_submit(x);

    return 0;
}