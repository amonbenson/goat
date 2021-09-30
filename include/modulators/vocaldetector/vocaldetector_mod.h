/**
 * @file vocaldetector_mod.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "control/manager.h"
#include "goat_config.h"
#include "pitch/vocaldetector.h"


/**
 * @struct vocaldetector_mod
 * @brief modulator wrapper around the global vocaldetector
 */
typedef struct {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg; /**< the global configuration */

    vocaldetector *vd; /**< the vocal detector to be used */
} vocaldetector_mod;


/**
 * @memberof vocaldetector_mod
 * @brief create a new vocaldetector_mod instance
 * 
 * @param cfg the global configuration
 * @param vd the vocaldetector to be used
 * @param name the name of the modulator
 * @return vocaldetector_mod* the new instance
 */
vocaldetector_mod *vdmod_new(goat_config *cfg, vocaldetector *vd, const char *name);

/**
 * @memberof vocaldetector_mod
 * @brief delete an existing vocaldetector_mod instance
 * 
 * @param vdmod the instance to be deleted
 */
void vdmod_free(vocaldetector_mod *vdmod);

/**
 * @memberof vocaldetector_mod
 * @brief update the vocaldetector modulator.
 * This will not invoke the vocaldetector's perform function.
 * 
 * @param vdmod the instance to be updated
 * @param in the input buffer
 * @param n the number of samples in the input buffer
 */
void vdmod_perform(vocaldetector_mod *vdmod, float *in, int n);
