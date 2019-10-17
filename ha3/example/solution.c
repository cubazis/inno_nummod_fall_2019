#include <stdio.h>
#include "blackbox.h"

int main() {
    int n;
    scanf("%d", &n);
    blackbox_init(n);

    double const a = -1;
    double const b = 1;
    double const err = 10e-10;

    double df4 = blackbox_df(4);
    int step_n = (int) ceil(pow(32.0*df4/(180*err), 0.25))+1;

    step_n = (step_n%2 != 0) ? step_n+1 : step_n;


    double h = 2.0/step_n;

    double sum = blackbox(a)+blackbox(b);

    for (int i = 1;i <= step_n-1; i++){
        sum += (i%2==0) ? 2*blackbox(a+i*h) : 4*blackbox(a+i*h);
    }
    sum *=h/3;


    printf("%.12lf\n",sum);


}