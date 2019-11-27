#include <math.h>
#include <stdlib.h>
#include "blackbox.h"

static const double eps = 1e-9;

double residual(const double *b, const double *x, const int n, double *res) {
    double ret = 0;
    blackbox_mult(x, res);
    for (int r = 0; r < n; ++r) {
        res[r] -= b[r];
        ret += res[r] * res[r];
    }
    return sqrt(ret);
}

int main() {
    blackbox_init();
    const int n = blackbox_size();

    double sum[n];
    double I[n];
    for (int i = 0; i < n; i++) I[i] = 1;
    blackbox_mult(I, sum);

    double max = sum[0];
    double min = sum[0];
    for (int i = 0; i < n; i++) {
        double a = fabs(sum[i]);
        if (a > max) max = a;
        if (a < min) min = a;
    }
    double c = 2.0 / (max + min);

    double b[n];
    double x[n];
    double d[n];
    blackbox_rhs(b);

    while (residual(b, x, n, d) > eps) {
        double xn[n];
        blackbox_mult(x, xn);
        for (int i = 0; i < n; i++)
            x[i] += c * (b[i] - xn[i]) ;
    }

    blackbox_submit(x);
    return 0;
}
