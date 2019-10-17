//
// Created by MFade on 10.10.2019.
//

#include <cstdio>
#include "blackbox.h"

#define a -1.0
#define b 1.0
#define e 1e-9

using namespace std;

//fast pow function
inline double fastPow(double value, double pow) {

    //define structures
    union {
        double d;
        int x[2];
    }
    u = { value };

    u.x[1] = (int)(pow * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;

    return u.d;
}

// middle point rule function
double midpoint_rule(int n){

    //create h and s
    double h = (b - a)/n;
    double s = 0;

    // run
    for (int i = 0; i < n; i++) {
        s += blackbox(a + h * (i + 0.5));
    }

    return s * h;
}


// simpson rule function
double simpson_rule(int n){

    //create h and sum variables
    double h = (b - a)/n;
    double s = blackbox(a) +blackbox(b);

    // run
    for (int i = 1; i <= n-1; i+=2) s += 4 * blackbox(a + h * i);
    for (int i = 2; i <= n-1; i+=2) s += 2 * blackbox(a + h * i);

    return s * h / 3;
}

int main() {

    int k;

    //read values
    scanf("%d", &k);

    //init blackbox for k
    blackbox_init(k);

    // get number points that will reach required error
    int nSimp = fastPow(blackbox_df(4) / (90 * e * 0.75), 0.25) + 1;
    if (nSimp % 2 != 0){
        nSimp = nSimp + 1;
    }

    int nMid = sqrt(blackbox_df(2) / (3 * e)) + 1;

    // optimisation
    if (nMid <= nSimp){
        printf("%.12lf\n", midpoint_rule(nMid));
    } else{
        printf("%.12lf\n", simpson_rule(nSimp));
    }
}