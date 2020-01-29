#include <cstdio>
#include "blackbox.h"

using namespace std;

double a = -1.0;
double b = 1.0;
double eps = 1e-9;

double simpson_rule(int n){
    double h = (b - a)/n;

    double sum = 0;
    sum += blackbox(a);
    sum += blackbox(b);

    for (int i = 1; i <= n-1; i++) {
        double k = i%2==1 ? 4 : 2;
        sum += k * blackbox(a + h*i);
    }
    return sum * h/3;
}

double midpoint_rule(int n){
    double h = (b - a)/n;

    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += blackbox(a + h * (i + 0.5));
    }

    return sum * h;
}

inline double fastPow(double a, double b) {
    union {
        double d;
        int x[2];
    } u = { a };
    u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    return u.d;
}

int main() {

    int k;
    scanf("%d", &k);

    blackbox_init(k);

    // get # points that will reach required error
    int N_simp = fastPow(blackbox_df(4) / (90 * eps*0.75), 0.25) + 1;
    N_simp = (N_simp % 2 != 0) ? (N_simp + 1 ) : N_simp;

    int N_mid = sqrt(blackbox_df(2) / (3 * eps)) + 1;

    // optimisation: we will calculate the on that requires less points
    double result = N_mid <= N_simp ? midpoint_rule(N_mid) : simpson_rule(N_simp);

    printf("%.12lf\n", result);
}