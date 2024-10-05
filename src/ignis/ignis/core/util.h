#ifndef IGNIS_MATH_UTIL_H
#define IGNIS_MATH_UTIL_H

#include "ignis/core/defines.h"
#include <stdlib.h>

#define PI 3.14159265358979323846f

#define _INTL_IMAX_BITS(m)                                                     \
    ((m) / ((m) % 255 + 1) / 255 % 255 * 8 + 7 - 86 / ((m) % 255 + 12))
#define _INTL_RAND_MAX_WIDTH _INTL_IMAX_BITS(RAND_MAX)

STATIC_ASSERT((RAND_MAX & (RAND_MAX + 1u)) == 0,
              "RAND_MAX not a Mersenne number");

FORCE_INLINE u64 rand_u64() {
    // https://stackoverflow.com/a/33021408
    u64 val = 0;
    for (i32 i = 0; i < 64; i += _INTL_RAND_MAX_WIDTH) {
        val <<= _INTL_RAND_MAX_WIDTH;
        val ^= (unsigned)rand();
    }
    return val;
}

FORCE_INLINE f64 round_up(f64 val) {
    usize intmax = ((INTMAX_MAX / 2 + 1) * 2.0);
    if (val >= INTMAX_MIN && val < intmax) {
        intmax_t i = (intmax_t)val;
        if (i < 0 || val == i) {
            return i;
        }
        return i + 1.0;
    }
    return val;
}

FORCE_INLINE f64 round_down(f64 val) {
    usize intmax = ((INTMAX_MAX / 2 + 1) * 2.0);
    if (val >= INTMAX_MIN && val < intmax) {
        intmax_t i = (intmax_t)val;
        if (i > 0 || val == i) {
            return i;
        }
        return i - 1.0;
    }
    return val;
}

#endif
