#pragma once

#include <inttypes.h>
#ifdef _MSC_VER
    # include <intrin.h>
    # include <nmmintrin.h>
#endif


// conversion type character for size_t
#if defined(_WIN64)
    #define PRI_SIZE_T PRIu64
#elif defined(_WIND32)
    #define PRI_SIZE_T PRIu32
#else
    #define PRI_SIZE_T "zu"
#endif


// general purpose functions
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define fastmod(n, m) ((n) & ((m) - 1)) // fast modulo function if m is a power of two
#define is_pwrtwo(n) ((n) != 0 && !((n) & ((n) - 1)))
