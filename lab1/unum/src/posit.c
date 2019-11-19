#include "posit.h"
#include <math.h>
#include <stdio.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define SHIFTR(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define SHIFTL(bits, shift) ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define LZ_COUNT(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))

#define POW2(n) (SHIFTL(1, (n)))

#define HIDDEN_BIT(a) ((a >> 1) | ms_bit)

#define bool short

#define _uint32 unsigned int

#define _uint64 unsigned long long

const _uint32 ms_bit = 1u << 31u;
const _uint32 MASK = 0xFFFFFFFF;
const int ES = 4;
const int POSIT_SIZE = 32;
const int EXP_MAX = 30 * 16;

typedef struct unpacked_t {
    bool sign;
    int EXP;
    _uint32 fraction;
} unpacked;

unpacked _toUnpacked(double f) {
    unpacked up;
    int bias = 1023;

    union {
        double f;
        _uint64 u;
    } un;
    un.f = f;
    up.sign = (bool) SHIFTR(un.u, 63u);
    up.EXP = (int) ((SHIFTR(un.u, 52u)) & 0x7FFu) - bias;
    up.fraction = (_uint32) (SHIFTR(SHIFTL(un.u, 12u), 32u));

    if (up.EXP == -bias) {
        up.EXP -= LZ_COUNT(up.fraction);
        up.fraction = SHIFTL(up.fraction, LZ_COUNT(up.fraction) + 1);
    }

    return up;
}

bool _is_nar(posit p) {
    return p.bits == ms_bit;
}

int _get_r_s(posit p) {
    int l_z = LZ_COUNT(SHIFTL(p.bits, 1));
    int l_o = LZ_COUNT(SHIFTL(~p.bits, 1));
    int r_s = MAX(l_z, l_o) + 1;

    return MIN(r_s, POSIT_SIZE - 1);
}


posit _toPosit(unpacked up) {
    if (up.EXP < -EXP_MAX)
        up.EXP = -EXP_MAX;
    else if (up.EXP > EXP_MAX)
        up.EXP = EXP_MAX;

    int reg = up.EXP / POW2(ES) - (up.EXP % POW2(ES) < 0);

    _uint32 s_s = 1;
    _uint32 r_s = MAX(-reg + 1, reg + 2);
    _uint32 EXP = up.EXP - POW2(ES) * reg;

    _uint32 regbits;
    if (reg < 0)
        regbits = SHIFTR(ms_bit, -reg);
    else
        regbits = SHIFTL(MASK, POSIT_SIZE - reg - 1);
    _uint32 EXPbits = SHIFTL(EXP, POSIT_SIZE - ES);


    _uint32 tmp = SHIFTR(regbits, s_s)
                  | SHIFTR(EXPbits, s_s + r_s)
                  | SHIFTR(up.fraction, s_s + r_s + ES);

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

    int r_s = _get_r_s(p);

    int l_z = LZ_COUNT(SHIFTL(p.bits, 1));
    int l_o = LZ_COUNT(SHIFTL(~p.bits, 1) | 1);

    int reg = (l_z == 0 ? l_o - 1 : -l_z);
    int EXP = SHIFTR(SHIFTL(p.bits, 1 + r_s), POSIT_SIZE - ES);

    res.EXP = POW2(ES) * reg + EXP;
    res.fraction = SHIFTL(p.bits, 1 + r_s + ES);

    return res;
}

double toDouble(posit a) {
    unpacked up = _fromPosit(a);

    int fexp = up.EXP + 1023;

    _uint64 fexpbits;
    _uint64 ffracbits;

    if (fexp > 2046) {
        fexpbits = SHIFTL((_uint64) 2046, 53);
        ffracbits = -1;
    } else if (fexp < 1) {
        fexpbits = 0;
        ffracbits = SHIFTL((_uint64)(ms_bit | SHIFTR(up.fraction, 1)), 64 - POSIT_SIZE);
        ffracbits = SHIFTR(ffracbits, -fexp);
    } else {
        fexpbits = SHIFTL((_uint64)(fexp & 0x7FF), 53);
        ffracbits = SHIFTL((_uint64) up.fraction, 64 - POSIT_SIZE);
    }

    union {
        double f;
        _uint64 u;
    } un;

    un.u = ffracbits;
    un.u = fexpbits | SHIFTR(un.u, 11);
    un.u = SHIFTL((_uint64) up.sign, 63) | SHIFTR(un.u, 1);

    if (SHIFTL(un.u, 1) == 0)
        un.u++;

    return un.f;
}

unpacked _add(unpacked a, unpacked b) {
    unpacked r;

    _uint64 a_frac = HIDDEN_BIT(a.fraction);
    _uint64 b_frac = HIDDEN_BIT(b.fraction);
    _uint64 frac;

    if (a.EXP > b.EXP) {
        r.EXP = a.EXP;
        b_frac = SHIFTR(b_frac, a.EXP - b.EXP);
    } else {
        r.EXP = b.EXP;
        a_frac = SHIFTR(a_frac, b.EXP - a.EXP);
    }

    frac = a_frac + b_frac;
    if (SHIFTR(frac, POSIT_SIZE) != 0) {
        r.EXP++;
        frac = SHIFTR(frac, 1);
    }

    r.sign = a.sign;
    r.fraction = SHIFTL(frac, 1);

    return r;
}

unpacked _sub(unpacked a, unpacked b) {
    unpacked r;
    _uint64 a_frac = HIDDEN_BIT(a.fraction);
    _uint64 b_frac = HIDDEN_BIT(b.fraction);
    _uint64 frac;
    r.sign = a.sign;
    if (a.EXP > b.EXP || (a.EXP == b.EXP && a.fraction > b.fraction)) {
        r.EXP = a.EXP;
        b_frac = SHIFTR(b_frac, a.EXP - b.EXP);
        frac = a_frac - b_frac;
    } else {
        r.sign = !a.sign;
        r.EXP = b.EXP;
        a_frac = SHIFTR(a_frac, b.EXP - a.EXP);
        frac = b_frac - a_frac;
    }


    r.EXP -= LZ_COUNT(frac);
    r.fraction = SHIFTL(frac, LZ_COUNT(frac) + 1);

    return r;
}


posit add(posit a, posit b) {
    unpacked unpacked_a = _fromPosit(a);
    unpacked unpacked_b = _fromPosit(b);

    if ((unpacked_a.EXP == unpacked_b.EXP && unpacked_a.fraction == unpacked_b.fraction) && (unpacked_a.sign + unpacked_b.sign) % 2 == 1)
        return toPosit(0);
    else if (unpacked_a.sign == unpacked_b.sign){
        return _toPosit(_add(unpacked_a, unpacked_b));}

    else
        return _toPosit(_sub(unpacked_a, unpacked_b));
}

posit subtract(posit a, posit b) {
    unpacked unpacked_a = _fromPosit(a);
    unpacked unpacked_b = _fromPosit(b);

    if ((unpacked_a.EXP == unpacked_b.EXP && unpacked_a.fraction == unpacked_b.fraction) && (unpacked_a.sign + unpacked_b.sign) % 2 == 0)
        return toPosit(0);
    else if (unpacked_a.sign == unpacked_b.sign)
        return _toPosit(_sub(unpacked_a, unpacked_b));
    else
        return _toPosit(_add(unpacked_a, unpacked_b));
}

posit multiply(posit a, posit b) {
    unpacked unpacked_a = _fromPosit(a);
    unpacked unpacked_b = _fromPosit(b);
    unpacked r;

    _uint64 a_frac = HIDDEN_BIT(unpacked_a.fraction);
    _uint64 b_frac = HIDDEN_BIT(unpacked_b.fraction);
    _uint32 frac = SHIFTR(a_frac * b_frac, POSIT_SIZE);
    int EXP = unpacked_a.EXP + unpacked_b.EXP + 1;

    if ((frac & ms_bit) == 0) {
        EXP--;
        frac = SHIFTL(frac, 1);
    }

    r.sign = unpacked_a.sign ^ unpacked_b.sign;
    r.EXP = EXP;
    r.fraction = SHIFTL(frac, 1);

    return _toPosit(r);
}


posit power(posit a, posit b) {
    return toPosit(pow(toDouble(a), toDouble(b)));
}

posit negate(posit a) {
    if ((int) a.bits < 0 && !_is_nar(a)) {
        posit res = {(~a.bits) + 1};
        return res;
    }
    posit res = {~(a.bits - 1)};
    return res;
}

posit posit_abs(posit a) {
    unpacked unpacked_a = _fromPosit(a);
    return unpacked_a.sign ? negate(a) : a;
}

bool is_less(posit a, posit b) {
    return toDouble(a) < toDouble(b);
}
