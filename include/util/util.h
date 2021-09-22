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
