//-DCMAKE_PREFIX_PATH=/home/{$USER}/lib/cmake/unum/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "math.h"
// Created by cubazis on 18.08.2019.
//

//
// Created by cubazis on 18.08.2019.
//

#ifndef UNUM_POSIT_H
#define UNUM_POSIT_H

// posit_c_api.h: generic C API defining the posit api
//
// Copyright (C) 2017-2019 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

// set up the correct C11 infrastructure




//////////////////////////////////////////////////////////////////////
/// Standard posit configuration per the POSIT standard
typedef union posit8_u   {
	uint8_t x[1];
	uint8_t v;
}							posit8_t;	// posit<8,0>
typedef union posit16_u  {
	uint8_t x[2];
	uint16_t v;
}							posit16_t;	// posit<16,1>
typedef union posit32_u {
	uint8_t x[4];
	uint32_t v;
}							posit32_t;	// posit<32,2>
typedef union posit64_u {
	uint8_t x[8];
	uint64_t v;
}							posit64_t;	// posit<64,3>
typedef union posit128_u {
	uint8_t x[16];
	uint64_t longs[2];
} 							posit128_t;	// posit<128,4>
typedef union posit256_u {
	uint8_t x[32];
	uint64_t longs[4];
}							posit256_t;	// posit<256,5>

// these storage formats are the same, but interpretation of the bits is specialized
typedef posit8_t posit8_0_t;
typedef posit8_t posit8_1_t;
typedef posit8_t posit8_2_t;
typedef posit8_t posit8_3_t;
typedef posit16_t posit16_0_t;
typedef posit16_t posit16_1_t;
typedef posit16_t posit16_2_t;
typedef posit16_t posit16_3_t;
typedef posit32_t posit32_0_t;
typedef posit32_t posit32_1_t;
typedef posit32_t posit32_2_t;
typedef posit32_t posit32_3_t;
typedef posit64_t posit64_0_t;
typedef posit64_t posit64_1_t;
typedef posit64_t posit64_2_t;
typedef posit64_t posit64_3_t;

///////////////////////////////////////////////////////////////////////
///   associated quire configurations
typedef union quire8_u {
	uint8_t x[8];
	uint64_t v;
}							quire8_t;	// quire<8,0,39>
typedef union quire16_u {
	uint8_t x[16];
	uint64_t v[2];
}							quire16_t;	// quire<16,1,15>
typedef union quire32_u {
	uint8_t x[64];
	uint64_t v[8];
}							quire32_t;	// quire<32,2,31>
typedef union quire64_u {
	uint8_t x[256];
	uint64_t v[32];
}							quire64_t;	// quire<64,3,63>
typedef union quire128_u {
	uint8_t x[1024];
	uint64_t v[128];
}							quire128_t;	// quire<128,4,127>
typedef union quire256_u {
	uint8_t x[4096];
	uint64_t v[512];
}							quire256_t; // quire<256,5,255>

///	quire<  8, 0, 7>      32 bits		<--- likely not enough capacity bits
///	quire< 16, 1, 15>    128 bits
///	quire< 32, 2, 31>    512 bits
///	quire< 64, 3, 63>   2048 bits
///	quire<128, 4, 127>  8192 bits		<--- likely too many capacity bits
///	quire<256, 5, 7>   32520 bits		<--- 4065 bytes: smallest size aligned to 4byte boundary
///	quire<256, 5, 255> 32768 bits       <--- 4096 bytes

//////////////////////////////////////////////////////////////////////
/// special posits

//////////////////////////////////////////////////////////////////////
// for Deep Learning/AI algorithms
typedef union posit4_u   {
	uint8_t x[1];
	uint8_t v;
} posit4_t;

// Important posit constants // we a storing this in little endian
static const posit4_t  NAR4  = {{
	                                0x08
                                }};
static const posit8_t  NAR8  = {{
	                                0x80
                                }};
static const posit16_t NAR16 = {{
	                                0x00, 0x80
                                }};
static const posit32_t NAR32 = {{
	                                0x00, 0x00, 0x00, 0x80
                                }};
static const posit64_t NAR64 = {{
	                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
                                }};
static const posit128_t NAR128 = {{
	                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
                                  }};
static const posit256_t NAR256 = {{
	                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
                                  }};

static const posit4_t   ZERO4   = {{ 0 }};
static const posit8_t   ZERO8   = {{ 0 }};
static const posit16_t  ZERO16  = {{ 0 }};
static const posit32_t  ZERO32  = {{ 0 }};
static const posit64_t  ZERO64  = {{ 0 }};
static const posit128_t ZERO128 = {{ 0 }};
static const posit256_t ZERO256 = {{ 0 }};

enum {
	// assume UTF-16 UNICODE strings: 2 bytes per character
	/// report posit format for posit4_t. str must be at least 7 characters in size:    4.0x4p + /0 is 7 chars
		posit4_str_SIZE = 7,
#define posit4_str_SIZE 2*posit4_str_SIZE

	/// report posit format for posit8_t. str must be at least 8 characters in size:    8.0x40p + /0 is 8 chars
		posit8_str_SIZE = 8,
#define posit8_str_SIZE 2*posit8_str_SIZE

	/// report posit format for posit16_t. str must be at least 11 characters in size:  16.1x4000p + /0 is 11 chars
		posit16_str_SIZE = 11,
#define posit16_str_SIZE 2*posit16_str_SIZE

	/// report posit format for posit32_t. str must be at least 15 characters in size:  32.2x40000000p + /0 is 15 chars
		posit32_str_SIZE = 16,
#define posit32_str_SIZE 2*posit32_str_SIZE

	/// report posit format for posit64_t. str must be at least 23 characters in size:  64.3x1234567812345678p + /0 is 23 chars
		posit64_str_SIZE = 23,
#define posit64_str_SIZE 2*posit64_str_SIZE

	/// report posit format for posit128_t. str must be at least 40 characters in size:  128.4x12345678123456781234567812345678p + /0 is 40 chars
		posit128_str_SIZE = 40,
#define posit128_str_SIZE 2*posit128_str_SIZE

	/// report posit format for posit128_t. str must be at least 40 characters in size:  128.4x1234567812345678123456781234567812345678123456781234567812345678p + /0 is 72 chars
		posit256_str_SIZE = 72
#define posit256_str_SIZE 2*posit256_str_SIZE
};

// reinterpret bits from an unsigned integer type to a posit type
static inline posit4_t   posit4_reinterpret(uint8_t n)   { posit4_t  x; x.v = n; return x; }
static inline posit8_t   posit8_reinterpret(uint8_t n)   { posit8_t  x; x.v = n; return x; }
static inline posit16_t  posit16_reinterpret(uint16_t n) { posit16_t x; x.v = n; return x; }
static inline posit32_t  posit32_reinterpret(uint32_t n) { posit32_t x; x.v = n; return x; }
static inline posit64_t  posit64_reinterpret(uint64_t n) { posit64_t x; x.v = n; return x; }

#if defined(__cplusplus) || defined(_MSC_VER)
static inline posit128_t posit128_reinterpret(uint64_t* n) {
    posit128_t out;
    out.longs[0] = n[0];
    out.longs[1] = n[1];
    return out;
}
static inline posit256_t posit256_reinterpret(uint64_t* n) {
    posit256_t out;
    out.longs[0] = n[0];
    out.longs[1] = n[1];
    out.longs[2] = n[2];
    out.longs[3] = n[3];
    return out;
}
#else
// static array parameters are illegal in C++ but they provide valuable verification in C
static inline posit128_t posit128_reinterpret(uint64_t n[static 2]) {
	return (posit128_t){ .longs = { n[0], n[1] } };
}
static inline posit256_t posit256_reinterpret(uint64_t n[static 4]) {
	return (posit256_t){ .longs = { n[0], n[1], n[2], n[3] } };
}
#endif

// And reinterpret the bits from a posit to an unsigned integer type (where possible)
static inline uint8_t   posit4_bits(posit4_t p)   { return p.v; }
static inline uint8_t   posit8_bits(posit8_t p)   { return p.v; }
static inline uint16_t  posit16_bits(posit16_t p) { return p.v; }
static inline uint32_t  posit32_bits(posit32_t p) { return p.v; }
static inline uint64_t  posit64_bits(posit64_t p) { return p.v; }
/**
#define POSIT_NBITS 4
#include "posit_c_macros.h"
#undef POSIT_NBITS

#define POSIT_NBITS 8
#include "posit_c_macros.h"
#undef POSIT_NBITS

#define POSIT_NBITS 16
#include "posit_c_macros.h"
#undef POSIT_NBITS

#define POSIT_NBITS 32
#include "posit_c_macros.h"
#undef POSIT_NBITS

#define POSIT_NBITS 64
#include "posit_c_macros.h"
#undef POSIT_NBITS

#define POSIT_NBITS 128
#include "posit_c_macros.h"
#undef POSIT_NBITS

#define POSIT_NBITS 256
#include "posit_c_macros.h"
#undef POSIT_NBITS
**/
static const uint8_t posit8_sign_mask = 0x80;

// characterization tests
bool posit8_isnar(posit8_t p) { return (p.v == 0x80); }
bool posit8_iszero(posit8_t p) { return (p.v == 0x00); }
bool posit8_isone(posit8_t p) { return (p.v == 0x40); }      // pattern 010000...
bool posit8_isminusone(posit8_t p) { return (p.v == 0xC0); } // pattern 110000...
bool posit8_isneg(posit8_t p) { return (p.v & 0x80); }
bool posit8_ispos(posit8_t p) { return !(p.v & 0x80); }
bool posit8_ispowerof2(posit8_t p) { return !(p.v & 0x1); }

// decode takes the raw bits of the posit, and returns the regime, m, and returns the fraction bits in 'remainder'
int8_t posit8_decode_regime(const uint8_t bits, uint8_t* remaining);

// rounding
inline uint8_t posit8_round(const int8_t m, uint16_t fraction);

//inline uint8_t posit8_roundDiv(const int8_t m, uint16_t fraction, bool nonZeroRemainder);

// conversion functions
int posit8_sign_value(posit8_t p) { return (p.v & 0x80 ? -1 : 1); }

float posit8_fraction_value(uint8_t fraction);

void posit8_checkExtraTwoBits(float f, float temp, bool* bitsNPlusOne, bool* bitsMore);

uint16_t posit8_convertFraction(float f, uint8_t fracLength, bool* bitsNPlusOne, bool* bitsMore);
// assignment operators for native types
posit8_t posit8_fromsi(int rhs);

posit8_t posit8_fromf(float f);

posit8_t posit8_fromd(double d);

posit8_t posit8_fromld(long double ld);

float posit8_tof(posit8_t p);

double posit8_tod(posit8_t p);

long double posit8_told(posit8_t p);

int posit8_tosi(posit8_t p);

// arithmetic operators
posit8_t posit8_negate(posit8_t p);

posit8_t posit8_addMagnitude(posit8_t lhs, posit8_t rhs);

posit8_t posit8_subMagnitude(posit8_t lhs, posit8_t rhs);

posit8_t posit8_addp8(posit8_t lhs, posit8_t rhs);

posit8_t posit8_subp8(posit8_t lhs, posit8_t rhs);

posit8_t posit8_mulp8(posit8_t lhs, posit8_t rhs);

posit8_t posit8_divp8(posit8_t lhs, posit8_t rhs);

posit8_t posit8_reciprocate(posit8_t rhs);

// posit - posit binary logic functions
bool posit8_equal(posit8_t lhs, posit8_t rhs);
bool posit8_notEqual(posit8_t lhs, posit8_t rhs);
bool posit8_lessThan(posit8_t lhs, posit8_t rhs);
bool posit8_greaterThan(posit8_t lhs, posit8_t rhs);
bool posit8_lessOrEqual(posit8_t lhs, posit8_t rhs);
bool posit8_greaterOrEqual(posit8_t lhs, posit8_t rhs);

posit8_t posit8_sqrt(posit8_t a);
posit8_t posit8_log(posit8_t a);
posit8_t posit8_exp(posit8_t a);
int posit8_cmp(posit8_t a, posit8_t b);
void posit8_str(char* str, posit8_t a);

#endif //UNUM_POSIT_H


// elementary functions
posit8_t posit8_sqrt(posit8_t a) {
	posit8_t p;
	float f = posit8_tof(a);
	float root = sqrtf(f);
	p = posit8_fromf(root);
	return p;
}
posit8_t posit8_log(posit8_t a) {
	posit8_t p;
	float f = posit8_tof(a);
	float logarithm = logf(f);
	p = posit8_fromf(logarithm);
	return p;
}
posit8_t posit8_exp(posit8_t a) {
	posit8_t p;
	float f = posit8_tof(a);
	float exponent = expf(f);
	p = posit8_fromf(exponent);
	return p;
}
// logic functions
// cmp returns -1 if a < b, 0 if a == b, and 1 if a > b
int posit8_cmp(posit8_t a, posit8_t b) {
	// posits are ordered as signed integers
	return a.v - b.v;
}

// string conversion functions
void posit8_str(char* str, posit8_t a) {
	float f = posit8_tof(a);
	sprintf(str, "%f", f);
}


// decode takes the raw bits of the posit, and returns the regime, m, and returns the fraction bits in 'remainder'
inline int8_t posit8_decode_regime(const uint8_t bits, uint8_t* remaining) {
	int8_t m = 0;
	*remaining = (bits << 2) & 0xFF;
	if (bits & 0x40) {  // positive regimes
		while (*remaining >> 7) {
			++m;
			*remaining = (*remaining << 1) & 0xFF;
		}
	}
	else {              // negative regimes
		m = -1;
		while (!(*remaining >> 7)) {
			--m;
			*remaining = (*remaining << 1) & 0xFF;
		}
		*remaining &= 0x7F;
	}
	return m;
}

// rounding
uint8_t posit8_round(const int8_t m, uint16_t fraction) {
	uint8_t scale, regime, bits;
	if (m < 0) {
		scale = (-m & 0xFF);
		regime = 0x40 >> scale;
	}
	else {
		scale = m + 1;
		regime = 0x7F - (0x7F >> scale);
	}

	if (scale > 6) {
		bits = m<0 ? 0x1 : 0x7F;  // minpos and maxpos
	}
	else {
		fraction = (fraction & 0x3FFF) >> scale;
		uint8_t final_fbits = (uint8_t)(fraction >> 8);
		bool bitNPlusOne = (bool)(0x80 & fraction);
		bits = (uint8_t)regime + (uint8_t)final_fbits;
		// n+1 frac bit is 1. Need to check if another bit is 1 too if not round to even
		if (bitNPlusOne) {
			uint8_t moreBits = (0x7F & fraction) ? 0x01 : 0x00;
			bits += (bits & 0x01) | moreBits;
		}
	}
	return bits;
}


uint8_t posit8_roundDiv(const int8_t m, uint16_t fraction, bool nonZeroRemainder) {
	uint8_t scale, regime, bits;
	if (m < 0) {
		scale = (-m & 0xFF);
		regime = 0x40 >> scale;
	}
	else {
		scale = m + 1;
		regime = 0x7F - (0x7F >> scale);
	}

	if (scale > 6) {
		bits = m<0 ? 0x1 : 0x7F;  // minpos and maxpos
	}
	else {
		// remove carry and rcarry bits and shift to correct position
		fraction &= 0x7F;
		uint8_t final_fbits = fraction >> (scale + 1);
		bool bitNPlusOne = (bool)(0x1 & (fraction >> scale));
		bits = (uint8_t)regime + (uint8_t)final_fbits;
		if (bitNPlusOne) {
			uint8_t moreBits = (((1 << scale) - 1) & fraction) ? 0x01 : 0x00;
			if (nonZeroRemainder) moreBits = 0x01;
			// n+1 frac bit is 1. Need to check if another bit is 1 too if not round to even
			bits += (bits & 0x01) | moreBits;
		}
	}
	return bits;
}

float posit8_fraction_value(uint8_t fraction) {
	float v = 0.0f;
	float scale = 1.0f;
	uint8_t mask = 0x80;
	for (int i = 5; i >= 0; i--) {
		if (fraction & mask) v += scale;
		scale *= 0.5f;
		mask >>= 1;
		if (scale == 0.0) break;
	}
	return v;
}

void posit8_checkExtraTwoBits(float f, float temp, bool* bitsNPlusOne, bool* bitsMore) {
	temp /= 2.0;
	if (temp <= f) {
		*bitsNPlusOne = 1;
		f -= temp;
	}
	if (f>0)
		*bitsMore = 1;
}

uint16_t posit8_convertFraction(float f, uint8_t fracLength, bool* bitsNPlusOne, bool* bitsMore) {

	uint_fast8_t frac = 0;

	if (f == 0) return 0;
	else if (f == INFINITY) return 0x80;

	f -= 1; //remove hidden bit
	if (fracLength == 0)
		posit8_checkExtraTwoBits(f, 1.0, bitsNPlusOne, bitsMore);
	else {
		float temp = 1;
		while (true) {
			temp /= 2;
			if (temp <= f) {
				f -= temp;
				fracLength--;
				frac = (frac << 1) + 1; //shift in one
				if (f == 0) {
					//put in the rest of the bits
					frac <<= (uint_fast8_t)fracLength;
					break;
				}

				if (fracLength == 0) {
					posit8_checkExtraTwoBits(f, temp, bitsNPlusOne, bitsMore);

					break;
				}
			}
			else {
				frac <<= 1; //shift in a zero
				fracLength--;
				if (fracLength == 0) {
					posit8_checkExtraTwoBits(f, temp, bitsNPlusOne, bitsMore);
					break;
				}
			}
		}
	}
	//printf("convertfloat: frac:%d bitsNPlusOne: %d, bitsMore: %d\n", frac, bitsNPlusOne, bitsMore);
	return frac;
}

posit8_t posit8_fromsi(int rhs) {
	// special case for speed as this is a common initialization
	posit8_t p = { { 0x00} };
	if (rhs == 0) {
		return p;
	}
	bool sign = (rhs < 0 ? true : false);
	int v = sign ? -rhs : rhs; // project to positive side of the projective reals
	uint8_t raw;
	if (v > 48) { // +-maxpos
		raw = 0x7F;
	}
	else if (v < 2) {
		raw = (v << 6);
	}
	else {
		uint8_t mask = 0x40;
		int8_t k = 6;
		uint8_t fraction_bits = (v & 0xff);
		while (!(fraction_bits & mask)) {
			k--;
			fraction_bits <<= 1;
		}
		fraction_bits = (fraction_bits ^ mask);
		raw = (0x7F ^ (0x3F >> k)) | (fraction_bits >> (k + 1));

		mask = 0x1 << k; //bitNPlusOne
		if (mask & fraction_bits) {
			if (((mask - 1) & fraction_bits) | ((mask << 1) & fraction_bits)) raw++;
		}
	}

	p.v = (sign ? -raw : raw);
	return p;
}

posit8_t posit8_fromf(float f) {
	posit8_t p = { { 0x00 } };
	bool sign;
	uint8_t k = 0;
	bool bitNPlusOne = 0, bitsMore = 0;
	const float _minpos = 0.015625f;
	const float _maxpos = 64.0f;

	sign = (f < 0.0 ? true : false);

	if (isinf(f) || isnan(f)) {
		p.v = 0x80;
	}
	else if (f == 0) {
		p.v = 0;
	}
	else if (f == 1) {
		p.v = 0x40;
	}
	else if (f == -1) {
		p.v = 0xC0;
		return p;
	}
	else if (f >= _maxpos) {
		p.v = 0x7F;
	}
	else if (f <= -_maxpos) {
		p.v = 0x81;
	}
	else if (f <= _minpos && !sign) {
		p.v = 0x01;
	}
	else if (f >= -_minpos && sign) {
		p.v = 0xFF;
	}
	else if (f > 1 || f < -1) {
		if (sign) {
			f = -f; // project to positive reals to simplify computation
		}
		k = 1; //because k = m-1; so need to add back 1
		if (f <= _minpos) {
			p.v = 0x01;
		}
		else {
			//regime
			while (f >= 2) {
				f *= 0.5;
				k++;
			}

			//rounding off regime bits
			if (k > 6)
				p.v = 0x7F;
			else {
				int8_t fracLength = 6 - k;
				uint8_t frac = (uint8_t)posit8_convertFraction(f, fracLength, &bitNPlusOne, &bitsMore);
				uint_fast8_t regime = 0x7F - (0x7F >> k);
				p.v = (regime + frac);
				if (bitNPlusOne) p.v += ((p.v & 1) | bitsMore);
			}
			p.v = (sign ? -p.v : p.v);
		}
	}
	else if (f < 1 || f > -1) {
		if (sign) {
			f = -f;
		}
		k = 0;
		while (f<1) {
			f *= 2;
			k++;
		}
		// rounding off regime bits
		if (k > 6)
			p.v = 0x1;
		else {
			int8_t fracLength = 6 - k;
			uint8_t frac = (uint8_t)posit8_convertFraction(f, fracLength, &bitNPlusOne, &bitsMore);
			uint8_t regime = 0x40 >> k;
			p.v = (regime + frac);
			if (bitNPlusOne) p.v += ((p.v & 1) | bitsMore);
		}
		p.v = (sign ? -p.v : p.v);
	}
	else {
		//NaR - for NaN, INF and all other combinations
		p = NAR8;
	}
	return p;
}

posit8_t posit8_fromd(double d) {
	return posit8_fromf((float)d);
}

posit8_t posit8_fromld(long double ld) {
	return posit8_fromf((float)ld);
}

float posit8_tof(posit8_t p) {
	if (p.v == 0) return 0.0f;
	if (p.v == 0x80) return NAN;   //  INFINITY is not semantically correct. NaR is Not a Real and thus is more closely related to a NAN, or Not a Number

	uint8_t bits = (p.v & 0x80 ? -p.v : p.v);  // use 2's complement when negative
	uint8_t fraction = 0;
	int8_t m = posit8_decode_regime(bits, &fraction);

	float s = (float)(posit8_sign_value(p));
	float r = (m > 0 ? (float)((uint32_t)(1) << m) : (1.0f / (float)((uint32_t)(1) << -m)));
	float e = 1.0f;
	float f = 1.0f;
	f += posit8_fraction_value(fraction);

//	printf("sign = %f : m = %d : regime = %f : fraction = 0x%x : fraction_value %f\n", s, m, r, fraction, f);
	return s * r * e * f;
}

double posit8_tod(posit8_t p) {
	return (double)posit8_tof(p);
}

long double posit8_told(posit8_t p) {
	return (long double)posit8_tof(p);
}

int posit8_tosi(posit8_t p) {
	if (posit8_isnar(p)) return (int)NAN; // INFINITY;
	return (int)(posit8_tof(p));
}

posit8_t posit8_negate(posit8_t p) {
	p.v = -p.v; // 0 and NaR are invariant under uint8 arithmetic
	return p;
}

posit8_t posit8_addMagnitude(posit8_t lhs, posit8_t rhs) {
	//	printf("lhs = 0x%02x  rhs = 0x%02x\n", (uint8_t)(lhs.v), (uint8_t)(rhs.v));
	posit8_t p = { { 0x80 } };
	// process special cases
	if (posit8_isnar(lhs) || posit8_isnar(rhs)) {   // NaR
		return p;
	}
	if (posit8_iszero(lhs) || posit8_iszero(rhs)) { // zero
		p.v = (uint8_t)(lhs.v | rhs.v);
		return p;
	}
	bool sign = (bool)(lhs.v & posit8_sign_mask);
	if (sign) {
		lhs.v = -lhs.v & 0xFF;
		rhs.v = -rhs.v & 0xFF;
	}
	if (lhs.v < rhs.v) {
		uint8_t tmp = lhs.v;
		lhs.v = rhs.v;
		rhs.v = tmp;
	}

	// decode the regimes and extract the fractions of the operands
	uint8_t remaining = 0;
	int8_t mA = posit8_decode_regime(lhs.v, &remaining);
	uint16_t lhs_fraction = (0x80 | remaining) << 7;
	int8_t mB = posit8_decode_regime(rhs.v, &remaining);
	uint16_t rhs_fraction = (0x80 | remaining) << 7;
	int8_t shiftRight = mA - mB; // calculate the shift to normalize the fractions

	if (shiftRight > 7) {  // catastrophic cancellation case
		rhs_fraction = 0;
	}
	else {
		rhs_fraction >>= shiftRight; // align the rhs fraction
	}
	uint16_t result_fraction = lhs_fraction + rhs_fraction; // add

	bool rcarry = (bool)(0x8000 & result_fraction); // is MSB set
	if (rcarry) {
		mA++;
		result_fraction >>= 1;
	}

	uint8_t raw = posit8_round(mA, result_fraction);
	p.v = (sign ? -raw : raw);
	return p;
}

posit8_t posit8_subMagnitude(posit8_t lhs, posit8_t rhs) {
	// process special cases
	posit8_t p = { { 0x80} };
	if (posit8_isnar(lhs) || posit8_isnar(rhs)) {
		return p;
	}
	if (posit8_iszero(lhs) || posit8_iszero(rhs)) {
		p.v = (uint8_t)(lhs.v | rhs.v);
		return p;
	}

	// Both operands are actually the same sign if rhs inherits sign of sub: Make both positive
	bool sign = (bool)(lhs.v & posit8_sign_mask);
	if (sign) {
		lhs.v = -lhs.v;
	}
	else {
		rhs.v = -rhs.v;
	}

	if (lhs.v == rhs.v) {
		p.v = 0;
		return p;
	}
	if (lhs.v < rhs.v) {
		uint8_t tmp = lhs.v;
		lhs.v = rhs.v;
		rhs.v = tmp;
		sign = !sign;
	}

	// decode the regimes and extract the fractions of the operands
	uint8_t remaining = 0;
	int8_t mA = posit8_decode_regime(lhs.v, &remaining);
	uint16_t lhs_fraction = (0x80 | remaining) << 7;
	int8_t mB = posit8_decode_regime(rhs.v, &remaining);
	uint16_t rhs_fraction = (0x80 | remaining) << 7;
	int8_t shiftRight = mA - mB;  // calculate the shift to normalize the fractions

	if (shiftRight >= 14) { // catastrophic cancellation case
		p.v = (sign ? -lhs.v : lhs.v);
		return p;
	}
	else {
		rhs_fraction >>= shiftRight;	// align the rhs fraction
	}
	uint16_t result_fraction = lhs_fraction - rhs_fraction;

	while ((result_fraction >> 14) == 0) {
		mA--;
		result_fraction <<= 1;
	}
	bool ecarry = (bool)(0x4000 & result_fraction);
	if (!ecarry) {
		mA--;
		result_fraction <<= 1;
	}

	uint8_t raw = posit8_round(mA, result_fraction);
	p.v = (sign ? -raw : raw);
	return p;
}

posit8_t posit8_addp8(posit8_t lhs, posit8_t rhs) {
	posit8_t p;
	if ((lhs.v^rhs.v) >> 7) {
		p = posit8_subMagnitude(lhs, rhs);
	}
	else {
		p = posit8_addMagnitude(lhs, rhs);
	}
	return p;
}

posit8_t posit8_subp8(posit8_t lhs, posit8_t rhs) {
	posit8_t p;
	bool differentSign = (bool)((lhs.v^rhs.v) >> 7);
	rhs.v = -rhs.v;
	if (differentSign) {
		p = posit8_addMagnitude(lhs, rhs);
	}
	else {
		p = posit8_subMagnitude(lhs, rhs);
	}
	return p;
}

posit8_t posit8_mulp8 (posit8_t lhs, posit8_t rhs) {
	// process special cases
	posit8_t p = { { 0x80} };
	if (posit8_isnar(lhs) || posit8_isnar(rhs)) {
		return p;
	}
	if (posit8_iszero(lhs) || posit8_iszero(rhs)) {
		p.v = 0;
		return p;
	}

	// calculate the sign of the result
	bool sign = (bool)(lhs.v & 0x80) ^ (bool)(rhs.v & 0x80);
	lhs.v = lhs.v & 0x80 ? -lhs.v : lhs.v;
	rhs.v = rhs.v & 0x80 ? -rhs.v : rhs.v;

	// decode the regimes and extract the fractions of the operands
	uint8_t remaining = 0;
	int8_t mA = posit8_decode_regime(lhs.v, &remaining);
	uint16_t lhs_fraction = (0x80 | remaining);
	int8_t mB = posit8_decode_regime(rhs.v, &remaining);
	uint16_t rhs_fraction = (0x80 | remaining);
	uint16_t result_fraction = (lhs_fraction * rhs_fraction);
	int8_t scale = mA + mB;

	bool rcarry = (bool)(result_fraction & 0x8000);
	if (rcarry) {
		scale++;
		result_fraction >>= 1;
	}

	// round
	uint8_t raw = posit8_round(scale, result_fraction);
	p.v = (sign ? -raw : raw);
	return p;
}

posit8_t posit8_divp8(posit8_t lhs, posit8_t rhs) {
	// process special cases
	posit8_t p = { { 0x80 } };
	if (posit8_isnar(lhs) || posit8_isnar(rhs) || posit8_iszero(rhs)) {
		return p;
	}
	if (posit8_iszero(lhs)) {
		p.v = 0;
		return p;
	}

	// calculate the sign of the result
	bool sign = (bool)(lhs.v & 0x80) ^ (bool)(rhs.v & 0x80);
	lhs.v = lhs.v & 0x80 ? -lhs.v : lhs.v;
	rhs.v = rhs.v & 0x80 ? -rhs.v : rhs.v;

	// decode the regimes and extract the fractions of the operands
	uint8_t remaining = 0;
	int8_t mA = posit8_decode_regime(lhs.v, &remaining);
	uint16_t lhs_fraction = (0x80 | remaining) << 7;
	int8_t mB = posit8_decode_regime(rhs.v, &remaining);
	uint16_t rhs_fraction = (0x80 | remaining);
	div_t result = div(lhs_fraction, rhs_fraction);
	uint16_t result_fraction = result.quot;
	uint16_t remainder = result.rem;
	int8_t scale = mA - mB;

	if (result_fraction != 0) {
		bool rcarry = result_fraction >> 7; // this is the hidden bit (7th bit) , extreme right bit is bit 0
		if (!rcarry) {
			--scale;
			result_fraction <<= 1;
		}
	}

	// round
	uint8_t raw = posit8_roundDiv(scale, result_fraction, remainder != 0);
	p.v = (sign ? -raw : raw);
	return p;
}

posit8_t posit8_reciprocate(posit8_t rhs) {
	posit8_t one = { { 0x40 } };
	return posit8_divp8(one, rhs);
}

bool posit8_equal(posit8_t lhs, posit8_t rhs){ return lhs.v == rhs.v;  }
bool posit8_notEqual(posit8_t lhs, posit8_t rhs){ return lhs.v != rhs.v;  }
bool posit8_lessThan(posit8_t lhs, posit8_t rhs){ return lhs.v < rhs.v; }
bool posit8_greaterThan(posit8_t lhs, posit8_t rhs){ return lhs.v > rhs.v;  }
bool posit8_lessOrEqual(posit8_t lhs, posit8_t rhs){ return lhs.v <= rhs.v; }
bool posit8_greaterOrEqual(posit8_t lhs, posit8_t rhs){ return lhs.v >= rhs.v; }

int ValidateMultiplication(void) {
	const int NR_POSITS = 256;
	int nrOfFailedTests = 0;
	posit8_t pa, pb, pc;

	nrOfFailedTests = 0;
	for (int a = 0; a < NR_POSITS; ++a) {
		pa = posit8_reinterpret(a);
		for (int b = 0; b < NR_POSITS; ++b) {
			pb = posit8_reinterpret(b);
			pc = posit8_mulp8(pa, pb);
			float da, db, dref;
			da = posit8_tof(pa);
			db = posit8_tof(pb);
			dref = da * db;
			posit8_t pref = posit8_fromf(dref);
			if (posit8_cmp(pref, pc)) {
				printf("FAIL: 8.0x%02xp - 8.0x%02xp produced 8.0x%02xp instead of 8.0x%02xp\n",
				       posit8_bits(pa), posit8_bits(pb), posit8_bits(pc), posit8_bits(pref));
				++nrOfFailedTests;
			}
		}
	}
	if (nrOfFailedTests) {
		printf("multiplication  FAIL\n");
	}
	else {
		printf("multiplication  PASS\n");
	}

	return nrOfFailedTests;
}

int main() {
	posit8_t pa, pb, pc;

	pa.v = 0x20;
	pb.v = 0x40;
	pc = posit8_mulp8(pa, pb);
	printf("%f + %f = %f (8.0x%02x)\n", posit8_tof(pa), posit8_tof(pb), posit8_tof(pc), posit8_bits(pc));

	ValidateMultiplication();
	//printf("sqrt(0) = %f\n", sqrt(0.0));
}