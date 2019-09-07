#include "packing_posit.h"

struct unpacked_t op2_add(struct unpacked_t a, struct unpacked_t b);
struct unpacked_t op2_sub(struct unpacked_t a, struct unpacked_t b);
struct unpacked_t op2_mul(struct unpacked_t a, struct unpacked_t b);
struct unpacked_t op2_div(struct unpacked_t a, struct unpacked_t b);
struct unpacked_t posit_sqrt(struct unpacked_t a);