#pragma once
#include "control/manager.h"
#include <stdlib.h>
#include <math.h>


typedef struct _rand_mod {
    control_modulator super;

    float   mu, sigma;
    float   rand_num;
    int     seed;
} rand_mod;


rand_mod *rand_mod_new(control_manager *mgr, const char *name);

void rand_mod_free(rand_mod *rm, control_manager *mgr);

void rand_mod_perform(rand_mod *rm, float *in, int n);

void rand_setSeed(rand_mod *rm);

float rand_nn(rand_mod *rm);