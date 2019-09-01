//
// Created by Timur on 29.08.2019.
//

#include "stdio.h"
#include "posit.h"

int main(void){
    posit myPosit = {
            0x7FFFFFFC
    };
    printf("%f", getValueInPosit32(myPosit));
    return 0;
}