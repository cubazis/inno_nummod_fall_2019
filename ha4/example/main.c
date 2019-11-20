#include <math.h>
#include <stdlib.h>
#include "blackbox.h"

static const double eps = 1e-9;


double dot_product(const double *a, const double *b, const int n) {
    double ret = 0;
    for (int i = 0; i < n; i++)
        ret += a[i] * b[i];

    return ret;
}



int main() {
    blackbox_init();
    const int n = blackbox_size();
    double *b = (double *) malloc((n+1)*sizeof(double));
    double *x = (double *) malloc((n+1)*sizeof(double));
    double *temp1 = (double *) malloc((n+1)*sizeof(double));
    double *r = (double *) malloc((n+1)*sizeof(double));
    double *r_old = (double *) malloc((n+1)*sizeof(double));
    double *z = (double *) malloc((n+1)*sizeof(double));


// ''Метод сопряжённых градиентов (для решения СЛАУ)'' http://bit.ly/cgm_SLAE
// initial contitions
    blackbox_rhs(b);
    for (int i = 0; i < n; i++) x[i] = 0;

    blackbox_mult(x, temp1); // temp1 = Ax
    for (int i = 0; i < n; i++){ // r = b - Ax, z = b - Ax
        r[i] = b[i] - temp1[i];
        z[i] = b[i] - temp1[i];
    }



//    rsnew = r' * r;
//    if sqrt(rsnew) < 1e-10
//    break;
    double alpha, betta;
    while (sqrt(dot_product(r,r, n)) > eps) {
//       calculating recursivetly


        blackbox_mult(z, temp1); // temp1 = Az_{k-1}
        alpha = dot_product(r, r, n) / dot_product(temp1, z, n);

//        x_k = x_{k-1} + alpha * z_{k-1}
        for (int i = 0; i < n; i++){ // r = b - Ax, z = b - Ax
            x[i] = x[i]  + alpha * z[i];
        }

//        r_k = r_{k-1} - alpha * Az_{k-1}
        for (int i = 0; i < n; i++){ // r = b - Ax, z = b - Ax
            r_old[i] = r[i];
            r[i] = r_old[i]  - (alpha * temp1[i]);
        }

        betta = dot_product(r, r, n) / dot_product(r_old, r_old, n);

//        z_k  = r_k + betta *  z_{k-1}
        for (int i = 0; i < n; i++){ // r = b - Ax, z = b - Ax
            z[i] = r[i]  + (betta * r_old[i]);
        }
    }

    blackbox_submit(x);
    return 0;
}
