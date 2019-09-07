//
// Created by cubazis on 18.08.2019.
//

#include "posit.h"
#include "utils.h"
#include <math.h>

const _uint32 MSB = 1u << 31u;

const _uint32 MASK = 0xFFFFFFFF;

const int ES = 2;

const int POSIT_SIZE = 32;

const int MAX_EXP = 120; // 2^ES * (POSIT_SIZE - 2)

typedef struct unpacked_t {
	bool neg;
	int exp;
	_uint32 frac;
} unpacked;

bool _is_zero(posit p)
{
	return p.bits == 0;
}

bool _is_nar(posit p)
{
	return p.bits == MSB;
}

bool _is_neg(posit p) {
	return (int) p.bits < 0 && !_is_nar(p);
}

unpacked _fromDouble(double f) {
	unpacked up;
	int bias = 1023;

	union {
		double f;
		_uint64 u;
	} un;

	un.f = f;
	up.neg = (bool) RSHIFT(un.u, 63u);
	up.exp = (int) ((RSHIFT(un.u, 52u)) & 0x7FFu) - bias;
	up.frac = (_uint32) (RSHIFT(LSHIFT(un.u, 12u), 32u));

	if (up.exp == -bias) {
		// normalize
		up.exp -= CLZ(up.frac);
		up.frac = LSHIFT(up.frac, CLZ(up.frac) + 1);
	}

	return up;
}

unpacked _fromPosit(posit p) {

	unpacked res;
	res.neg = _is_neg(p);

	if (res.neg) {
		p.bits = (~p.bits) + 1;
	}

	int lz = CLZ(LSHIFT(p.bits, 1));
	int lo = CLZ(LSHIFT(~p.bits, 1) | 1);
	int rs = MIN(MAX(lz, lo) + 1, POSIT_SIZE - 1);

	int reg = (lz == 0 ? lo - 1 : -lz);
	int exp = RSHIFT(LSHIFT(p.bits, 1 + rs), POSIT_SIZE - ES);

	res.exp = POW2(ES) * reg + exp;
	res.frac = LSHIFT(p.bits, 1 + rs + ES);

	return res;
}

posit _toPosit(unpacked up) {

	if (up.exp < -MAX_EXP) {
		up.exp = -MAX_EXP;
	} else if (up.exp > MAX_EXP) {
		up.exp = MAX_EXP;
	}

	int reg = FLOORDIV(up.exp, POW2(ES));
	_uint32 rs = MAX(-reg + 1, reg + 2);
	_uint32 exp = up.exp - POW2(ES) * reg;

	_uint32 regbits;
	if (reg < 0) {
		regbits = RSHIFT(MSB, -reg);
	} else {
		regbits = LSHIFT(MASK, POSIT_SIZE - reg - 1);
	}
	_uint32 expbits = LSHIFT(exp, POSIT_SIZE - ES);


	_uint32 tmp = RSHIFT(regbits, 1) | RSHIFT(expbits, 1 + rs) | RSHIFT(up.frac, 1 + rs + ES);

	if (up.neg) {
		tmp = ~(tmp - 1);
	}

	posit res= {tmp};
	return res;
}

posit fromDouble(double f) {
	return _toPosit(_fromDouble(f));
}

double toDouble(posit a) {
	unpacked up = _fromPosit(a);

	int fexp = up.exp + 1023;

	_uint64 fexpbits;
	_uint64 ffracbits;

	if (fexp > 2046) {
		fexpbits = LSHIFT((_uint64)2046, 53);
		ffracbits = -1;
	} else if (fexp < 1) {
		fexpbits = 0;
		ffracbits = LSHIFT((_uint64)(MSB | RSHIFT(up.frac, 1)), 64 - POSIT_SIZE);
		ffracbits = RSHIFT(ffracbits, -fexp);
	} else {
		fexpbits = LSHIFT((_uint64)(fexp & 0x7FF), 53);
		ffracbits = LSHIFT((_uint64)up.frac, 64 - POSIT_SIZE);
	}

	union {
		double f;
		_uint64 u;
	} un;

	un.u = ffracbits;
	un.u = fexpbits | RSHIFT(un.u, 11);
	un.u = LSHIFT((_uint64)up.neg, 63) | RSHIFT(un.u, 1);

	if (LSHIFT(un.u, 1) == 0) {
		un.u++;
	}

	return un.f;
}

unpacked _add(unpacked a, unpacked b, bool neg) {
	unpacked r;

	_uint64 afrac = HIDDEN_BIT(a.frac);
	_uint64 bfrac = HIDDEN_BIT(b.frac);
	_uint64 frac;

	if (a.exp > b.exp) {
		r.exp = a.exp;
		bfrac = RSHIFT(bfrac, a.exp - b.exp);
	} else {
		r.exp = b.exp;
		afrac = RSHIFT(afrac, b.exp - a.exp);
	}

	frac = afrac + bfrac;
	if (RSHIFT(frac, POSIT_SIZE) != 0) {
		r.exp++;
		frac = RSHIFT(frac, 1);
	}

	r.neg = neg;
	r.frac = LSHIFT(frac, 1);

	return r;
}

unpacked _sub(unpacked a, unpacked b, bool neg) {
	unpacked r;

	_uint64 afrac = HIDDEN_BIT(a.frac);
	_uint64 bfrac = HIDDEN_BIT(b.frac);
	_uint64 frac;

	if (a.exp > b.exp || (a.exp == b.exp && a.frac > b.frac)) {
		r.exp = a.exp;
		bfrac = RSHIFT(bfrac, a.exp - b.exp);
		frac = afrac - bfrac;
	} else {
		neg = !neg;
		r.exp = b.exp;
		afrac = RSHIFT(afrac, b.exp - a.exp);
		frac = bfrac - afrac;
	}

	r.neg = neg;
	r.exp -= CLZ(frac);
	r.frac = LSHIFT(frac, CLZ(frac) + 1);

	return r;
}

bool _is_opposite(unpacked a, unpacked b, bool neg) {
	return ((a.exp == b.exp && a.frac == b.frac) && ((neg && (a.neg == b.neg)) || (!neg && (a.neg != b.neg)) ));
}

posit add(posit ap, posit bp) {
	unpacked a = _fromPosit(ap);
	unpacked b = _fromPosit(bp);

	if (_is_opposite(a, b, 0))
		return fromDouble(0);

	if (a.neg == b.neg) {
		return _toPosit(_add(a, b, a.neg));
	} else {
		return _toPosit(_sub(a, b, a.neg));
	}
}

posit sub(posit ap, posit bp) {
	unpacked a = _fromPosit(ap);
	unpacked b = _fromPosit(bp);

	if (a.neg == b.neg) {
		return _toPosit(_sub(a, b, a.neg));
	} else {
		return _toPosit(_add(a, b, a.neg));
	}
}

posit multiply(posit ap, posit bp) {
	unpacked a = _fromPosit(ap);
	unpacked b = _fromPosit(bp);
	unpacked r;

	_uint64 afrac = HIDDEN_BIT(a.frac);
	_uint64 bfrac = HIDDEN_BIT(b.frac);
	_uint32 frac = RSHIFT(afrac * bfrac, POSIT_SIZE);
	int exp = a.exp + b.exp + 1;

	if ((frac & MSB) == 0) {
		exp--;
		frac = LSHIFT(frac, 1);
	}

	r.neg = a.neg ^ b.neg;
	r.exp = exp;
	r.frac = LSHIFT(frac, 1);

	return _toPosit(r);
}

posit power(posit a, posit b) {
	return fromDouble(pow(toDouble(a), toDouble(b)));
}

posit negate(posit a) {
	if (_is_neg(a)) {
		posit res = {(~a.bits) + 1};
		return res;
	}
	posit res = {~(a.bits - 1)};
	return res;
}

int test_function() {
	return 42;
}