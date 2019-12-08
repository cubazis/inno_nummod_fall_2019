//
// Created by Aine on 07.09.2019.
//

#include "posit.h"
#include <stdio.h>

struct posit32* init_va(int a){
    struct posit32* result = calloc(6, sizeof(struct posit32));
    result[0]=to_posit32(pow(10, a));
    result[1]=to_posit32(1223.0);
    result[2]=to_posit32(pow(10, a-1));
    result[3]=to_posit32(pow(10, a-2));
    result[4]=to_posit32(3.0);
    result[5]=to_posit32(-pow(10, a-5));

    return result;
}

struct posit32* init_vb(int b){
    struct posit32* result = calloc(6, sizeof(struct posit32));
    result[0]=to_posit32(pow(10, b));
    result[1]=to_posit32(2);
    result[2]=to_posit32(-pow(10, b+1));
    result[3]=to_posit32(pow(10, b));
    result[4]=to_posit32(2111);
    result[5]=to_posit32(pow(10, b+3));
    return result;
}

int main(){
    struct posit32 dotres;
    double sum_of_errors = 0.0;
    for (int a=5;a<=10;a+=5){
        for (int b=1;b<6;b++){
             dotres = dot_product(6,init_va(a),init_vb(b));
             //printf("a = %d, b = %d, dot = %f\n",a,b,to_double(dotres));
             sum_of_errors += fabs(to_double(dotres) - 8779.0);
        }
    }
    printf("%d",sum_of_errors == 0.0);
}