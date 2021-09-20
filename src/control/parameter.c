#include "control/parameter.h"
#include <stdlib.h>
#include <stdio.h>
#include "util/util.h"


control_parameter *control_parameter_new(const char *name, float default_value, float min, float max) {
    control_parameter *p = malloc(sizeof(control_parameter));
    if (p == NULL) return NULL;
    
    p->name = name;
    p->offset = default_value;
    p->min = min;
    p->max = max;
    p->next = NULL;

    for (int i = 0; i < CONTROL_NUM_SLOTS; i++) {
        p->slots[i].mod = NULL;
        p->slots[i].amount = 1.0f;
    }

    return p;
}

void control_parameter_free(control_parameter *p) {
    free(p);
}


static int control_parameter_validate_slot(size_t slot) {
    int valid = slot < CONTROL_NUM_SLOTS;
    if (!valid) fprintf(stderr, "control_parameter_add_modulator: slot %" PRI_SIZE_T " out of range\n", slot);
    return valid;
}

void control_parameter_add_modulator(control_parameter *p,
        size_t slot,
        control_modulator *mod) {
    if (!control_parameter_validate_slot(slot)) return;

    if (slot >= CONTROL_NUM_SLOTS) return;
    p->slots[slot].mod = mod;
}

void control_parameter_remove_modulator(control_parameter *p,
        size_t slot) {
    if (!control_parameter_validate_slot(slot)) return;

    p->slots[slot].mod = NULL;
}

void control_parameter_set_amount(control_parameter *p,
        size_t slot,
        float amount) {
    if (!control_parameter_validate_slot(slot)) return;

    p->slots[slot].amount = amount;
}


float control_parameter_get_float(control_parameter *p) {
    return p->value;
}

int control_parameter_get_int(control_parameter *p) {
    return (int) p->value;
}
