/**
 * @file lfo.h
 * @author Amon Benson, Kai, Valentin Lux
 * @brief 
 * @version 0.1
 * @date 2021-09-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "goat_config.h"
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


#define LFO_CURVE_SINE 0 /**< sin shaped curve */
#define LFO_CURVE_TRIANGLE 1 /**< triangle shaped curve */
#define LFO_CURVE_SQUARE 2 /**< square shaped curve */
#define LFO_CURVE_SAWTOOTH 3 /**< rising sawtooth curve */
#define LFO_CURVE_SAWTOOTH_REVERSED 4 /**< falling sawtooth curve */
#define LFO_NUM_CURVES 5 /**< total number of curves available */


/**
 * @struct low_frequency_oscillator
 * @brief a generic lfo modulator.
 */
typedef struct {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg; /**< the goat configuration */

    float phase; /**< the current phase of the lfo */
    control_parameter *frequency; /**< the frequency of the lfo */
    control_parameter *curve; /**< the curve type of the lfo */
} low_frequency_oscillator;


/**
 * @memberof low_frequency_oscillator
 * @brief create a new lfo modulator.
 * 
 * @param cfg the global goat configuration
 * @param name the name of the lfo
 * @return low_frequency_oscillator* a pointer to the new lfo or NULL if the allocation failed
 */
low_frequency_oscillator *lfo_new(goat_config *cfg, const char *name);

/**
 * @memberof low_frequency_oscillator
 * @brief free a lfo modulator.
 * 
 * @param lfo a reference to the lfo to free
 */
void lfo_free(low_frequency_oscillator *lfo);

/**
 * @memberof low_frequency_oscillator
 * @brief run the lfo modulator for a block of samples
 * 
 * @param lfo a reference to the lfo
 * @param in a reference to the input buffer
 * @param n the number of samples to process
 */
void lfo_perform(low_frequency_oscillator *lfo, float *in, int n);
