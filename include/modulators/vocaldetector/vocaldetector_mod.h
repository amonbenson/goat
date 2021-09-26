#pragma once

#include "control/manager.h"
#include "goat_config.h"
#include "pitch/vocaldetector.h"


typedef struct vocaldetector_mod {
    control_modulator super; /**< the modulator super class instance */
    goat_config *cfg; /**< the global configuration */

    vocaldetector *vd; /**< the vocal detector to be used */
} vocaldetector_mod;


vocaldetector_mod *vdmod_new(goat_config *cfg, vocaldetector *vd, const char *name);

void vdmod_free(vocaldetector_mod *vdmod);

void vdmod_perform(vocaldetector_mod *vdmod, float *in, int n);
