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
    double *b = (double *) malloc(n*sizeof(double));
    double *x = (double *) malloc(n*sizeof(double));
    double *d = (double *) malloc(n*sizeof(double));
    blackbox_rhs(b);
    for (int r = 0; r < n; r++) x[r] = 0;
    while (residual(b, x, n, d) > eps) {
	for (int r = 0; r < n; ++r) {
	    x[r] = /*...*/;
	}
    }
    blackbox_submit(x);
    printf("%.12lf\n", x);
    return 0;

	
}
