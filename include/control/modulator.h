#pragma once

#include <stddef.h>


struct control_modulator;
typedef void (*control_modulator_perform_method)(struct control_modulator *mod, float *in, int n);

typedef struct control_modulator {
    const char *name;
    control_modulator_perform_method perform_method;

    float value;

    struct control_modulator *next;
} control_modulator;


control_modulator *control_modulator_new(const char *name,
        control_modulator_perform_method perform_method,
        size_t subclass_size);

void control_modulator_free(control_modulator *modulator);
