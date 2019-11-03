#include <math.h>
#include "blackbox.h"

static const double eps = 1e-9;
#define N 10000

double residual(const double *b, const double *x, const int n, double res[N]) {
    double ret = 0;
    blackbox_mult(x, res);
    for (int r = 0; r < n; ++r) {
        res[r] -= b[r];
        ret += res[r] * res[r];
    }
    return sqrt(ret);
}

void it(const double b[N], double x[N], int n, double omega) {
    double xn[N];
    blackbox_mult(x, xn);

    for (int i = 0; i < n; i++) {
        x[i] += (b[i] - xn[i]) * omega;
    }
}

int main() {
    blackbox_init();
    double res[N], b[N], x[N], ones[N], sums[N];
    const int n = blackbox_size();

    for (int i = 0; i < n; i++) {
        ones[i] = 1;
    }

    blackbox_mult(ones, sums);
    double max_l = sums[0], min_l = sums[0];

    for (int i = 0; i < n; i++) {
        double a = fabs(sums[i]);
        if (a > max_l) max_l = a;
        if (a < min_l) min_l = a;
    }

    double omega = 2.0 / (max_l + min_l);
    blackbox_rhs(b);

    while (residual(b, x, n, res) > eps) {
        for (int i = 0; i < 10; i++) {
            it(b, x, n, omega);
        }
    }

    blackbox_submit(x);
    return 0;
}
