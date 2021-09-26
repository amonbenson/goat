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
