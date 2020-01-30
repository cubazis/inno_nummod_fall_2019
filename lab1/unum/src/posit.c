#include "posit.h"
#include <math.h>
#include <stdio.h>

// definitions
#define min(x, y) ((x) < (y) ? (x) : (y))

#define max(x, y) ((x) > (y) ? (x) : (y))

#define shiftR(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define shiftL(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define lz_count(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define pow_2(n) (shiftL(1, (n)))

#define hidden_bit(a) ((a >> 1) | ms_bit)

// define types
#define bool short
#define _uint32 unsigned int
#define _uint64 unsigned long long

// define constants
const _uint32 ms_bit = 1u << 31u;
const _uint32 mask = 0xFFFFFFFF;
const int es = 4;
const int p_size = 32;
const int exp_max = 30 * 16;

typedef struct unpacked_t {
    bool sign;
    int exp;
    _uint32 fraction;
} unpacked;

int _get_rs(posit p) {
    int lz = lz_count(shiftL(p.bits, 1));
    int lo = lz_count(shiftL(~p.bits, 1));
    int rs = max(lz, lo) + 1;

    return min(rs, p_size - 1);
}

bool _is_nar(posit p) {
    return p.bits == ms_bit;
}

unpacked _toUnpacked(double f) {
    unpacked up;
    int bias = 1023;

    union {
        double f;
        _uint64 u;
    } un;

    un.f = f;
    up.sign = (bool) shiftR(un.u, 63u);
    up.exp = (int) ((shiftR(un.u, 52u)) & 0x7FFu) - bias;
    up.fraction = (_uint32) (shiftR(shiftL(un.u, 12u), 32u));

    if (up.exp == -bias) {
        up.exp -= lz_count(up.fraction);
        up.fraction = shiftL(up.fraction, lz_count(up.fraction) + 1);
    }

    return up;
}

posit _toPosit(unpacked up) {
    if (up.exp < -exp_max)
        up.exp = -exp_max;
    else if (up.exp > exp_max)
        up.exp = exp_max;

    int reg = up.exp / pow_2(es) - (up.exp % pow_2(es) < 0);

    _uint32 ss = 1;
    _uint32 rs = max(-reg + 1, reg + 2);
    _uint32 exp = up.exp - pow_2(es) * reg;

    _uint32 regbits;
    if (reg < 0)
        regbits = shiftR(ms_bit, -reg);
    else
        regbits = shiftL(mask, p_size - reg - 1);
    _uint32 expbits = shiftL(exp, p_size - es);


    _uint32 tmp = shiftR(regbits, ss)
                  | shiftR(expbits, ss + rs)
                  | shiftR(up.fraction, ss + rs + es);

    if (up.sign)
        tmp = ~(tmp - 1);

    posit res = {tmp};
    return res;
}

posit toPosit(double f) {
    return _toPosit(_toUnpacked(f));
}

unpacked _fromPosit(posit p) {
    unpacked res;
    res.sign = (int) p.bits < 0 && !_is_nar(p);

    if (res.sign)
        p.bits = (~p.bits) + 1;

    int rs = _get_rs(p);

    int lz = lz_count(shiftL(p.bits, 1));
    int lo = lz_count(shiftL(~p.bits, 1) | 1);

    int reg = (lz == 0 ? lo - 1 : -lz);
    int exp = shiftR(shiftL(p.bits, 1 + rs), p_size - es);

    res.exp = pow_2(es) * reg + exp;
    res.fraction = shiftL(p.bits, 1 + rs + es);

    return res;
}

double toDouble(posit a) {
    unpacked up = _fromPosit(a);

    int fexp = up.exp + 1023;

    _uint64 fexpbits;
    _uint64 ffracbits;

    if (fexp > 2046) {
        fexpbits = shiftL((_uint64) 2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = shiftL((_uint64)(ms_bit | shiftR(up.fraction, 1)), 64 - p_size);
        ffracbits = shiftR(ffracbits, -fexp);
    } else {
        fexpbits = shiftL((_uint64)(fexp & 0x7FF), 53);
        ffracbits = shiftL((_uint64) up.fraction, 64 - p_size);
    }

    union {
        double f;
        _uint64 u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | shiftR(un.u, 11);
    un.u = shiftL((_uint64) up.sign, 63) | shiftR(un.u, 1);

    if (shiftL(un.u, 1) == 0)
        un.u++;

    return un.f;
}

unpacked _add(unpacked a, unpacked b) {
    unpacked r;

    _uint64 afrac = hidden_bit(a.fraction);
    _uint64 bfrac = hidden_bit(b.fraction);
    _uint64 frac;

    if (a.exp > b.exp) {
        r.exp = a.exp;
        bfrac = shiftR(bfrac, a.exp - b.exp);
    } else {
        r.exp = b.exp;
        afrac = shiftR(afrac, b.exp - a.exp);
    }

    frac = afrac + bfrac;
    if (shiftR(frac, p_size) != 0) {
        r.exp++;
        frac = shiftR(frac, 1);
    }

    r.sign = a.sign;
    r.fraction = shiftL(frac, 1);

    return r;
}

unpacked _sub(unpacked a, unpacked b) {
    unpacked r;
    _uint64 afrac = hidden_bit(a.fraction);
    _uint64 bfrac = hidden_bit(b.fraction);
    _uint64 frac;
    r.sign = a.sign;
    if (a.exp > b.exp || (a.exp == b.exp && a.fraction > b.fraction)) {
        r.exp = a.exp;
        bfrac = shiftR(bfrac, a.exp - b.exp);
        frac = afrac - bfrac;
    } else {
        r.sign = !a.sign;
        r.exp = b.exp;
        afrac = shiftR(afrac, b.exp - a.exp);
        frac = bfrac - afrac;
    }


    r.exp -= lz_count(frac);
    r.fraction = shiftL(frac, lz_count(frac) + 1);

    return r;
}


posit add(posit a, posit b) {
    unpacked u_a = _fromPosit(a);
    unpacked u_b = _fromPosit(b);

    if ((u_a.exp == u_b.exp && u_a.fraction == u_b.fraction) && (u_a.sign + u_b.sign) % 2 == 1)
        return toPosit(0);
    else if (u_a.sign == u_b.sign){
        return _toPosit(_add(u_a, u_b));}

    else
        return _toPosit(_sub(u_a, u_b));
}

posit subtract(posit a, posit b) {
    unpacked u_a = _fromPosit(a);
    unpacked u_b = _fromPosit(b);

    if ((u_a.exp == u_b.exp && u_a.fraction == u_b.fraction) && (u_a.sign + u_b.sign) % 2 == 0)
        return toPosit(0);
    else if (u_a.sign == u_b.sign)
        return _toPosit(_sub(u_a, u_b));
    else
        return _toPosit(_add(u_a, u_b));
}

posit multiply(posit a, posit b) {
    unpacked u_a = _fromPosit(a);
    unpacked u_b = _fromPosit(b);
    unpacked r;

    _uint64 afrac = hidden_bit(u_a.fraction);
    _uint64 bfrac = hidden_bit(u_b.fraction);
    _uint32 frac = shiftR(afrac * bfrac, p_size);
    int exp = u_a.exp + u_b.exp + 1;

    if ((frac & ms_bit) == 0) {
        exp--;
        frac = shiftL(frac, 1);
    }

    r.sign = u_a.sign ^ u_b.sign;
    r.exp = exp;
    r.fraction = shiftL(frac, 1);

    return _toPosit(r);
}


posit power(posit a, posit b) {
    return toPosit(pow(toDouble(a), toDouble(b)));
}

posit negate(posit p) {
    if ((int) p.bits < 0 && !_is_nar(p)) {
        posit res = {(~p.bits) + 1};
        return res;
    }
    posit res = {~(p.bits - 1)};
    return res;
}

posit posit_abs(posit a) {
    unpacked u_a = _fromPosit(a);
    return u_a.sign ? negate(a) : a;
}

bool is_less(posit a, posit b) {
    return toDouble(a) < toDouble(b);
}
