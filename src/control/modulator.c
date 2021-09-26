#include "control/modulator.h"
#include <stdlib.h>
#include <string.h>


control_modulator *control_modulator_new(const char *name,
        control_modulator_perform_method perform_method,
        size_t subclass_size) {
    control_modulator *m = (control_modulator *) malloc(subclass_size);
    if (m == NULL) return NULL;

    m->name = strdup(name);
    m->perform_method = perform_method;
    m->value = 0.0f;
    m->next = NULL;

    return m;
}

void control_modulator_free(control_modulator *m) {
    free(m->name);
    free(m);
}
