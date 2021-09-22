/**
 * @file goat.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief Main GOAT class
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "goat_config.h"
#include "granular/granular.h"
#include "scheduler/scheduler.h"
#include "modulators/modulator_bank.h"
#include "control/manager.h"

#include "params.h"

/**
 * @brief Main GOAT class
 */
typedef struct goat {
    goat_config config; /**< configuration parameters */
    modulator_bank *modbank; /**< the modulator bank */
    control_manager *mgr; /**< the control manager */
    granular *gran;     /**< the granular instance */
    scheduler *schdur;  /**< the scheduler instance */
    // analyzer *anlyz;    /**< the analyzer instance */
    // transformer *trans; /**< the transformer instance */
} goat;


/**
 * @memberof goat
 * @brief create a new goat instance
 * 
 * @return goat* the new goat instance
 */
goat *goat_new(goat_config *config);

/**
 * @memberof goat
 * @brief free a goat instance
 * 
 * @param g the goat instance to free
 */
void goat_free(goat *g);

/**
 * @memberof goat
 * @brief process a block of samples
 * 
 * @param g the goat instance
 * @param in the input samples
 * @param out the output samples
 * @param n the number of samples
 */
void goat_perform(goat *g, float *in, float *out, int n);
