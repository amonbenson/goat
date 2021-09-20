#pragma once
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


typedef struct low_frequency_oscillator {
    control_modulator super;

    float phase;
    float frequency;
} low_frequency_oscillator;


low_frequency_oscillator *lfo_new(control_manager *mgr, const char *name);

void lfo_free(low_frequency_oscillator *lfo, control_manager *mgr);

void lfo_perform(low_frequency_oscillator *lfo, float *in, int n);
