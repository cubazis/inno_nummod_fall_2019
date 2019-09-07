#ifndef __POSIT_PACK_H
#define __POSIT_PACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define POSIT_WIDTH     32
#define POSIT_ZERO      ((uint32_t)0x00000000)
#define POSIT_ONE       ((uint32_t)0x40000000)
#define POSIT_NAR       ((uint32_t)0x80000000)
#define POSIT_MSB       ((uint32_t)0x80000000)
#define POSIT_MASK      ((uint32_t)0xFFFFFFFF)

struct unpacked_t
{
    bool neg;
    int32_t exp;
    uint32_t frac;
};

uint32_t pack_posit(struct unpacked_t up, int nbits, int es);
float pack_float(struct unpacked_t up);
double pack_double(struct unpacked_t up);

struct unpacked_t unpack_posit(uint32_t p, int nbits, int es);
struct unpacked_t unpack_float(float f);
struct unpacked_t unpack_double(double f);

#ifdef __cplusplus
}
#endif

#endif
