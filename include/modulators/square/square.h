#pragma once
#include "goat_config.h"
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


/**
 * @struct square_modulator
 * @brief a generic square modulator.
 */
typedef struct square_modulator {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg; /**< the goat configuration */

    float phase; /**< the current phase of the square */
    control_parameter *frequency; /**< the frequency of the square */
} square_modulator;


/**
 * @memberof square_modulator
 * @brief create a new square modulator.
 * 
 * @param cfg the global goat configuration
 * @param name the name of the square
 * @return square_modulator* a pointer to the new square or NULL if the allocation failed
 */
square_modulator *square_new(goat_config *cfg, const char *name);

/**
 * @memberof square_modulator
 * @brief free a square modulator.
 * 
 * @param square a reference to the square to free
 */
void square_free(square_modulator *square);

/**
 * @memberof square_modulator
 * @brief run the square modulator for a block of samples
 * 
 * @param square a reference to the square
 * @param in a reference to the input buffer
 * @param n the number of samples to process
 */
void square_perform(square_modulator *square, float *in, int n);
