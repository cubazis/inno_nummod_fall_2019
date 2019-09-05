//
// Created by Timur on 29.08.2019.
//

#include "stdio.h"
#include "posit.h"

#define pos_dva 0x48000000
#define pos_tri 0x4C000000
#define pos_shest 0x54000000

int main(void){
    posit dva = {pos_dva };
    posit tri = {pos_tri };
    posit shest = { pos_shest };
    printf("Eto dva: %f\n", getValueInPosit32(&dva));
    printf("Eto tri: %f\n", getValueInPosit32(&tri));
    multiply(&dva, &tri);
    printf("Eto shest: %f\n", getValueInPosit32(&shest));
    return 0;
}