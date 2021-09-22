#pragma once
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


/**
 * @struct low_frequency_oscillator
 * @brief a generic lfo modulator.
 */
typedef struct low_frequency_oscillator {
    control_modulator super; /**< the modulator super class instance */

    float phase; /**< the current phase of the lfo */
    float frequency; /**< the frequency of the lfo */
} low_frequency_oscillator;


/**
 * @memberof low_frequency_oscillator
 * @brief create a new lfo modulator.
 * 
 * @param mgr a reference to the control manager
 * @param name the name of the lfo
 * @return low_frequency_oscillator* a pointer to the new lfo or NULL if the allocation failed
 */
low_frequency_oscillator *lfo_new(control_manager *mgr, const char *name);

/**
 * @memberof low_frequency_oscillator
 * @brief free a lfo modulator.
 * 
 * @param lfo a reference to the lfo to free
 * @param mgr a reference to the control manager
 */
void lfo_free(low_frequency_oscillator *lfo, control_manager *mgr);

/**
 * @memberof low_frequency_oscillator
 * @brief run the lfo modulator for a block of samples
 * 
 * @param lfo a reference to the lfo
 * @param in a reference to the input buffer
 * @param n the number of samples to process
 */
void lfo_perform(low_frequency_oscillator *lfo, float *in, int n);
