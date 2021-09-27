#pragma once
#include "goat_config.h"
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


/**
 * @struct triangle_modulator
 * @brief a generic triangle modulator.
 */
typedef struct triangle_modulator {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg; /**< the goat configuration */

    float phase; /**< the current phase of the triangle */
    control_parameter *frequency; /**< the frequency of the triangle */
} triangle_modulator;


/**
 * @memberof triangle_modulator
 * @brief create a new triangle modulator.
 * 
 * @param cfg the global goat configuration
 * @param name the name of the triangle
 * @return triangle_modulator* a pointer to the new triangle or NULL if the allocation failed
 */
triangle_modulator *triangle_new(goat_config *cfg, const char *name);

/**
 * @memberof triangle_modulator
 * @brief free a triangle modulator.
 * 
 * @param triangle a reference to the triangle to free
 */
void triangle_free(triangle_modulator *triangle);

/**
 * @memberof triangle_modulator
 * @brief run the triangle modulator for a block of samples
 * 
 * @param triangle a reference to the triangle
 * @param in a reference to the input buffer
 * @param n the number of samples to process
 */
void triangle_perform(triangle_modulator *triangle, float *in, int n);
