/**
 * @file util.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief general util functions
 * @version 0.1
 * @date 2021-07-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <math.h>
#include <inttypes.h>
#ifdef _MSC_VER
    # include <intrin.h>
    # include <nmmintrin.h>
#endif


/**
 * @def PRI_SIZE_T
 * @brief generic format specifier for `size_t` types
 */
#if defined(_WIN64)
    #define PRI_SIZE_T PRIu64
#elif defined(_WIND32)
    #define PRI_SIZE_T PRIu32
#else
    #define PRI_SIZE_T "zu"
#endif


// builtin functions
#if defined(__GNUC__)
    #define __util_popcount __builtin_popcount
    #define __util_popcountl __builtin_popcountl
    #define __util_popcountll __builtin_popcountll

    #define __util_clz __builtin_clz
    #define __util_clzl __builtin_clzl
    #define __util_clzll __builtin_clzll
#elif defined(_MSC_VER)
    #define __util_popcount _mm_popcnt_u32
    #define __util_popcountl _mm_popcnt_u64
    #define __util_popcountll _mm_popcnt_u64

    #define __util_clz __lzcnt
    #define __util_clzl __lzcnt64
    #define __util_clzll __lzcnt64
#else
    #error Unsupported compiler
#endif

/**
 * @def util_popcount(x)
 * @brief use the buildin function to count the number of set bits in @a x
 */
#define util_popcount(x) (_Generic((x), \
    unsigned int: __util_popcount, \
    unsigned long: __util_popcountl, \
    unsigned long long: __util_popcountll)(x))

/**
 * @def util_clz(x)
 * @brief use the buildin function to compute the number of leading zeros in @a x
 */
#define util_clz(x) (_Generic((x), \
    unsigned int: __util_clz, \
    unsigned long: __util_clzl, \
    unsigned long long: __util_clzll)(x))


/**
 * @def min(a, b)
 * @brief computes the minimum of @a a and @a b
 */
#define min(a, b) ((a) < (b) ? (a) : (b))

/**
 * @def max(a, b)
 * @brief computes the minimum of @a a and @a b
 */
#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * @def is_pwrtwo(n)
 * @brief checks if @a n is a power of two. @a n must be unsigned.
 */
#define is_pwrtwo(n) ((n) != 0 && !((n) & ((n) - 1)))

/**
 * @def next_pwrtwo(n)
 * @brief find the next power of two greater than or equal to @a n
 */
#define next_pwrtwo(n) ((n) == 1 ? 1 : (__typeof__(n)) 1 << (sizeof(n) * 8 - util_clz((n) - 1)))


#ifndef M_PI
    #define M_PI 3.14159265358979323846264338327950288 /**< pi */
#endif


/**
 * @def emod(a, b)
 * @brief Euclidean modulo
 * Result is always in the range [0, b), even if a < 0
 */
#define emod(a, b) ((a) < 0 ? ((a) % (b) + (b)) % (b) : (a) % (b))


/**
 * @def semitonefact(semitone)
 * @brief Convert a semitone number to a frequency factor
 * a semitone of -12.0f will result in a factor of 0.5f
 * and a semitone of +12.0f will result in a factor of 2.0f
 */
#define semitonefact(semitone) (pow(2, (semitone) / 12.0f))
