#pragma once

#include <stddef.h>
#include "uthash/utlist.h"


#define CONTROL_SLOTS 3


typedef float (*modulator_perform_method)(void *data, float *in, int n);

typedef struct modulator {
    modulator_perform_method perform_method;
    void *data;

    float value;

    struct modulator *next;
} modulator;

typedef struct control {
    modulator *mods[CONTROL_SLOTS];

    float amounts[CONTROL_SLOTS];
    float offset;
    float value;

    struct control *next;
} control;

typedef struct control_network {
    control *controls;
    modulator *modulators;
} control_network;


control_network *ctrlnet_new();

void ctrlnet_free(control_network *cn);


modulator *ctrlnet_add_modulator(control_network *cn, modulator_perform_method perform_method, void *data);

void ctrlnet_remove_modulator(control_network *cn, modulator *mod);

control *ctrlnet_add_control(control_network *cn);

void ctrlnet_remove_control(control_network *cn, control *ctrl);


void ctrlnet_add_modulator_to_control(control_network *cn, control *ctrl, modulator *mod);


void ctrlnet_perform(control_network *cn, float *in, int n);
