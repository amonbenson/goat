#pragma once

#include <stddef.h>
#include "uthash/utlist.h"
#include "control/parameter.h"
#include "control/modulator.h"


#define CONTROL_SLOTS 3


typedef struct control_manager {
    control_parameter *parameters;
    control_modulator *modulators;
} control_manager;


control_manager *control_manager_new();

void control_manager_free(control_manager *mgr);

control_parameter *control_manager_parameter_add(control_manager *mgr,
    const char *name,
    float default_value,
    float min,
    float max);

void control_manager_parameter_remove(control_manager *mgr, control_parameter *p);

control_parameter *control_manager_parameter_by_name(control_manager *mgr, const char *name);


control_modulator *control_manager_modulator_add(control_manager *mgr,
    const char *name,
    control_modulator_perform_method perform_method,
    size_t subclass_size);

void control_manager_modulator_remove(control_manager *mgr, control_modulator *m);

control_modulator *control_manager_modulator_by_name(control_manager *mgr, const char *name);


void control_manager_perform(control_manager *mgr, float *in, int n);
