#pragma once
#include "goat_config.h"
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


/**
 * @struct sawtooth_modulator
 * @brief a generic sawtooth modulator.
 */
typedef struct sawtooth_modulator {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg; /**< the goat configuration */

    float phase; /**< the current phase of the saw */
    control_parameter *frequency; /**< the frequency of the saw */
} sawtooth_modulator;


/**
 * @memberof sawtooth_modulator
 * @brief create a new sawtooth modulator.
 * 
 * @param cfg the global goat configuration
 * @param name the name of the saw
 * @return sawtooth_modulator* a pointer to the new saw or NULL if the allocation failed
 */
sawtooth_modulator *saw_new(goat_config *cfg, const char *name);

/**
 * @memberof sawtooth_modulator
 * @brief free a sawtooth modulator.
 * 
 * @param saw a reference to the saw to free
 */
void saw_free(sawtooth_modulator *saw);

/**
 * @memberof sawtooth_modulator
 * @brief run the sawtooth modulator for a block of samples
 * 
 * @param saw a reference to the saw
 * @param in a reference to the input buffer
 * @param n the number of samples to process
 */
void saw_perform(sawtooth_modulator *saw, float *in, int n);
