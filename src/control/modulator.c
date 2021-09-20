#include "control/modulator.h"
#include <stdlib.h>


control_modulator *control_modulator_new(const char *name, control_modulator_perform_method perform_method) {
    control_modulator *m = (control_modulator *) malloc(sizeof(control_modulator));
    if (m == NULL) return NULL;

    m->name = name;
    m->perform_method = perform_method;
    m->value = 0.0f;
    m->next = NULL;

    return m;
}

void control_modulator_free(control_modulator *m) {
    free(m);
}
