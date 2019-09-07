#define CLZ(n) ((n) == 0 ? 8 * sizeof(n) : __builtin_clz(n))
#define _uint64 unsigned long long
#define _uint32 unsigned int
#define bool short

#define MIN(a, b) \
    ((a) < (b) ? (a) : (b))

#define MAX(a, b) \
    ((a) > (b) ? (a) : (b))

#define LSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) << (shift))

#define RSHIFT(bits, shift) \
    ((shift) >= (int)(8 * sizeof(bits)) ? 0 : (bits) >> (shift))

#define POW2(n) \
    (LSHIFT(1, (n)))

#define FLOORDIV(a, b) \
    ((a) / (b) - ((a) % (b) < 0))

#define HIDDEN_BIT(a) \
    ((a >> 1) | MSB)
