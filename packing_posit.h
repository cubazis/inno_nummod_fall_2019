#include <stdint.h>
#include <stdbool.h>

#ifndef packing_posit
#define packing_posit

//posit types and constants
#define lutype    uint64_t
#define utype     uint32_t
#define zero_const      ((utype)0x00000000)
#define one_const       ((utype)0x40000000)
#define nar_const       ((utype)0x80000000)
#define msb_const       ((utype)0x80000000)
#define posit_mask      ((utype)0xFFFFFFFF)
#define POSIT_STYPE     int32_t
#define POSIT_WIDTH     32

struct unpacked_t
{
    bool neg;
    POSIT_STYPE exp;
    utype frac;
};

utype pack_posit(struct unpacked_t up, int nbits, int es);
float pack_float(struct unpacked_t up);
double pack_double(struct unpacked_t up);

struct unpacked_t unpack_posit(utype p, int nbits, int es);
struct unpacked_t unpack_float(float f);
struct unpacked_t unpack_double(double f);

#endif