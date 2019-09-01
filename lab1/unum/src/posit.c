//
// Created by cubazis on 18.08.2019.
//

#include <math.h>
#include <stdlib.h>
#include "posit.h"

double getValueInPosit32(posit posit32)
{
    uint32_t unum = posit32.value;
    int es = 2;
    int useed = (int)(pow(2, pow(2, es)));
    int sign = (unum & 0x80000000) >> 0x1F;
    unum <<= 0x1;
    if (sign == 0x1)
    {
        unum = ~(unum) + 0x1;
    }
    int regime_bit = (unum & 0x80000000) >> 0x1F;
    int useed_power = 0;
    int rs = 0;
    int current_bit = regime_bit;
    while (current_bit == regime_bit)
    {
        unum <<= 0x1;
        current_bit = (unum & 0x80000000) >> 0x1F;
        useed_power++;
        rs++;
    }
    useed_power--;
    rs--;
    if (regime_bit == 0x0)
    {
        useed_power = -(useed_power + 1);
    }
    unum <<= 0x1;
    int es_real = 32 - 1 - abs(rs + 1) - 1 < es ? 32 - 1 - abs(rs + 1) - 1 : es;
    uint8_t e = 0x0;
    int i;
    for (i = 0; i < es_real; i++)
    {
        current_bit = (unum & 0x80000000) >> 0x1F;
        e <<= 0x1;
        e += current_bit;
        unum <<= 0x1;
    }
    int fs = 32 - 1 - abs(rs + 1) - 1 - es_real;
    uint32_t f = 0x0;
    for (i = 0; i < fs; i++)
    {
        current_bit = (unum & 0x80000000) >> 0x1F;
        f <<= 0x1;
        f += current_bit;
        unum <<= 0x1;
    }
    return pow(-1, sign) * pow(useed, useed_power) * pow(2, e) * (1 + f / (pow(2, fs)));
}
