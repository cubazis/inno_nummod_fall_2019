#include "math.h"
#include "posit.h"
#include "posit_utils.h"
#include "posit_conversion.h"
#include "../utils.h"

int posit_compare(posit_unpack_t v1, posit_unpack_t v2) {
    if (v1.sign == 0 && v2.sign == 1) {
        return 1;
    }
    if (v1.sign == 1 && v2.sign == 0) {
        return -1;
    }

    int scale1 = calc_posit_scale(v1);
    int scale2 = calc_posit_scale(v2);

    if (scale1 > scale2) {
        return 1;
    }
    if (scale1 < scale2) {
        return -1;
    }

    if (scale1 == scale2) {
        if (v1.fraction > v2.fraction) {
            return 1;
        }
        if (v1.fraction < v2.fraction) {
            return -1;
        }
    }

    return 0;
}

int posit_is_less(posit_unpack_t v1, posit_unpack_t v2) {
    return posit_compare(v1, v2) == -1;
}

posit_unpack_t __add_similar(posit_unpack_t v1, posit_unpack_t v2) {
    posit_unpack_t vn;

    int scale1 = calc_posit_scale(v1);
    int scale2 = calc_posit_scale(v2);

    uint64_t M1 = fraction_with_hb(v1.fraction);
    uint64_t M2 = fraction_with_hb(v2.fraction);

    // adjust exponents
    int scalen = MAX(scale1, scale2);
    M1 = RSHIFT(M1, scalen - scale1);
    M2 = RSHIFT(M2, scalen - scale2);

    uint64_t Mn = M1 + M2;
    if (RSHIFT(Mn, POSIT32_U_SIZE) != 0) {
        scalen += 1;
        Mn = RSHIFT(Mn, 1);
    }

    vn = parse_scale(scalen, POSIT32_ES_SIZE);

    vn.sign = v1.sign;
    vn.fraction = fraction_remove_hn(Mn);

    return vn;
}

posit_unpack_t __sub_similar(posit_unpack_t v1, posit_unpack_t v2) {
    posit_unpack_t vn;

    int scale1 = calc_posit_scale(v1);
    int scale2 = calc_posit_scale(v2);

    uint64_t M1 = fraction_with_hb(v1.fraction);
    uint64_t M2 = fraction_with_hb(v2.fraction);

    // adjust exponents
    int scalen = MAX(scale1, scale2);
    M1 = RSHIFT(M1, scalen - scale1);
    M2 = RSHIFT(M2, scalen - scale2);

    // Decide on new sign and operands order
    uint8_t sign = v1.sign;
    uint64_t Mn;
    if (posit_compare(v1, v2) == 1) {
        Mn = M1 - M2;
    } else {
        sign = INVERT_BOOL(sign);
        Mn = M2 - M1;
    }

    int empty_bits = CLZ(Mn);

    vn = parse_scale(scalen - empty_bits, POSIT32_ES_SIZE);
    vn.sign = sign;
    vn.fraction = LSHIFT(fraction_remove_hn(Mn), empty_bits);

    return vn;
}

posit_unpack_t __mul_posit(posit_unpack_t v1, posit_unpack_t v2) {
    posit_unpack_t vn;

    uint64_t M1 = fraction_with_hb(v1.fraction);
    uint64_t M2 = fraction_with_hb(v2.fraction);
    uint32_t Mn = RSHIFT(M1 * M2, POSIT32_U_SIZE);

    int scale1 = calc_posit_scale(v1);
    int scale2 = calc_posit_scale(v2);
    int scalen = scale1 + scale2 + 1;

    int empty_bits = CLZ(Mn);
    scalen -= empty_bits;
    Mn = LSHIFT(Mn, empty_bits);

    vn = parse_scale(scalen, POSIT32_ES_SIZE);
    vn.sign = v1.sign ^ v2.sign;
    vn.fraction = fraction_remove_hn(Mn);

    return vn;
}

posit_unpack_t posit_neg(posit_unpack_t v) {
    if (is_zero(v)) {
        return v;
    }
    if (is_nar(v)) {
        return v;
    }

    v.sign = INVERT_BOOL(v.sign);

    return v;
}

posit_unpack_t posit_abs(posit_unpack_t v) {
    v.sign = 0;

    return v;
}

posit_unpack_t posit_add(posit_unpack_t v1, posit_unpack_t v2) {
    if (is_zero(v1)) {
        return v2;
    }
    if (is_zero(v2)) {
        return v1;
    }
    if (is_nar(v1) || is_nar(v2)) {
        return get_posit_nar();
    }

    if (v1.sign != v2.sign) {
        return __sub_similar(v1, posit_neg(v2));
    } else {
        return __add_similar(v1, v2);
    }
}

posit_unpack_t posit_sub(posit_unpack_t v1, posit_unpack_t v2) {
    if (is_zero(v1)) {
        return v2;
    }
    if (is_zero(v2)) {
        return v1;
    }
    if (is_nar(v1) || is_nar(v2)) {
        return get_posit_nar();
    }

    if (v1.sign != v2.sign) {
        return __add_similar(v1, posit_neg(v2));
    } else {
        return __sub_similar(v1, v2);
    }
}

posit_unpack_t posit_mul(posit_unpack_t v1, posit_unpack_t v2) {
    if (is_nar(v1) || is_nar(v2)) {
        return get_posit_nar();
    }
    if (is_zero(v1) || is_zero(v2)) {
        return get_posit_zero();
    }

    return __mul_posit(v1, v2);
}

posit_unpack_t posit_div(posit_unpack_t v1, posit_unpack_t v2) {
    posit_unpack_t vn = {0, 0, 0, 0};

    // todo

    return vn;
}

posit_unpack_t posit_pow(posit_unpack_t v, uint32_t pow) {
    return from_double(to_double(v), to_double(pow));
}

posit_unpack_t posit_dot_product(posit_unpack_t *left, posit_unpack_t *right, int size) {
    posit_unpack_t res = get_posit_zero();

    for (int i = 0; i < size; i++) {
        posit_unpack_t iter_res = posit_mul(left[i], right[i]);

        res = posit_add(res, iter_res);
    }

    return res;
}
