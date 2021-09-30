#include "control/manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


control_manager *control_manager_new() {
    control_manager *mgr = (control_manager *) malloc(sizeof(control_manager));
    if (mgr == NULL) return NULL;

    mgr->parameters = NULL;
    mgr->modulators = NULL;

    return mgr;
}

void control_manager_free(control_manager *mgr) {
    control_parameter *p, *ptmp;
    control_modulator *m, *mtmp;

    LL_FOREACH_SAFE(mgr->parameters, p, ptmp) {
        control_parameter_free(p);
    }

    LL_FOREACH_SAFE(mgr->modulators, m, mtmp) {
        control_modulator_free(m);
    }

    free(mgr);
}

control_parameter *control_manager_parameter_add(control_manager *mgr,
        const char *name,
        float default_value,
        float min,
        float max) {
    control_parameter *p = control_parameter_new(name, default_value, min, max);
    if (p == NULL) return NULL;

    LL_APPEND(mgr->parameters, p);

    return p;
}

void control_manager_parameter_remove(control_manager *mgr, control_parameter *p) {
    LL_DELETE(mgr->parameters, p);
    control_parameter_free(p);
}

control_parameter *control_manager_parameter_by_name(control_manager *mgr, const char *name) {
    control_parameter *p;
    LL_FOREACH(mgr->parameters, p) {
        if (strcmp(p->name, name) == 0) return p;
    }

    return NULL;
}


control_modulator *control_manager_modulator_add(control_manager *mgr,
        const char *name,
        control_modulator_perform_method perform_method,
        size_t subclass_size) {
    control_modulator *m = control_modulator_new(name, perform_method, subclass_size);
    if (m == NULL) return NULL;

    LL_APPEND(mgr->modulators, m);

    return m;
}

void control_manager_modulator_remove(control_manager *mgr, control_modulator *m) {
    LL_DELETE(mgr->modulators, m);
    control_modulator_free(m);
}

control_modulator *control_manager_modulator_by_name(control_manager *mgr, const char *name) {
    control_modulator *m;
    LL_FOREACH(mgr->modulators, m) {
        if (strcmp(m->name, name) == 0) return m;
    }

    return NULL;
}


void control_manager_perform(control_manager *mgr, float *in, int n) {
    control_modulator *m;
    control_parameter *p;
    int i;
    float v;

    // update all modulators
    LL_FOREACH(mgr->modulators, m) {
        m->perform_method(m, in, n);
    }

    // update all parameters
    LL_FOREACH(mgr->parameters, p) {
        v = p->offset;

        // apply each modulator slot weighted by its amount
        for (i = 0; i < CONTROL_NUM_SLOTS; i++) {
            if (p->slots[i].mod != NULL) {
                v += p->slots[i].amount * p->slots[i].mod->value;
            }
        }

        if (v < p->min) v = p->min;
        if (v > p->max) v = p->max;

        p->value = v;
    }
}
