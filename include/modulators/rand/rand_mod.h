#pragma once
#include "control/manager.h"
#include "goat_config.h"
#include <stdlib.h>
#include <math.h>

/**
 * @struct rand_mod
 * @brief random number generator for modulation
 */

typedef struct _rand_mod {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg;  /**< the goat configuration */

    control_parameter   *mu;
    control_parameter   *sigma; /**< mu: Expectation value; sigma: standard deviation*/
    control_parameter   *freq; /**< frequency of new random numbers*/
    float   rand_num; /**< a normal distributed random number*/
    int     seed; /**< Seed for the standard rand() function; needed for receiving different random numbers every time*/
    float   time; /**< time passed since last generated random number*/
} rand_mod;


/**
 * @memberof rand_mod
 * @brief create a new random modulator.
 * 
 * @param cfg the global goat configuration
 * @param name the name of the modulator
 * @return rand_mod* a pointer to the new rand modulator or NULL if the allocation failed
 */
rand_mod *rand_mod_new(goat_config *cfg, const char *name);

/**
 * @memberof rand_mod
 * @brief free a random modulator.
 * 
 * @param rm a reference to the random modulator to free
 */
void rand_mod_free(rand_mod *rm);

/**
 * @memberof rand_mod
 * @brief run the random modulator for a block of samples
 * 
 * @param rm a reference to the ramdom modulator
 * @param in a reference to the input buffer
 * @param n the number of samples to process
 */
void rand_mod_perform(rand_mod *rm, float *in, int n);

/**
 * @memberof rand_mod
 * @brief set the seed for random number generation.
 * @details without setting the seed one would get the same random numbers within a second
 * So this is needed for receiving different random numbers, even when it is called several times a second
 * 
 * @param rm a reference to the random modulator
 */
void rand_setSeed(rand_mod *rm);

/**
 * @memberof rand_mod
 * @brief Leva algorithm for normal distributed random numbers
 * @see https://dl.acm.org/doi/10.1145/138351.138364
 * 
 * @param rm a reference to the ramdom modulator; contains expectation value and standard deviation
 * @returns a float number out of a normal distribution
 */
float rand_nn(rand_mod *rm);