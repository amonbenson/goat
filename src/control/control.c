#include "control/control.h"

#include <stdlib.h>
#include <stdio.h>


control_network *ctrlnet_new() {
    control_network *cn = (control_network *) malloc(sizeof(control_network));
    if (cn == NULL) return NULL;

    cn->controls = NULL;
    cn->modulators = NULL;

    return cn;
}

void ctrlnet_free(control_network *cn) {
    // TODO: free controls and modulators

    free(cn);
}

modulator *ctrlnet_add_modulator(control_network *cn, modulator_perform_method perform_method, void *data) {
    modulator *mod = (modulator *) malloc(sizeof(modulator));
    if (mod == NULL) return NULL;

    mod->perform_method = perform_method;
    mod->data = data;

    LL_APPEND(cn->modulators, mod);
    return mod;
}

void ctrlnet_remove_modulator(control_network *cn, modulator *mod) {
    LL_DELETE(cn->modulators, mod);
    free(mod);
}

control *ctrlnet_add_control(control_network *cn) {
    control *ctrl = (control *) malloc(sizeof(control));
    if (ctrl == NULL) return NULL;

    ctrl->value = 0.0f;
    for (int i = 0; i < CONTROL_SLOTS; i++) {
        ctrl->amounts[i] = 1.0f;
        ctrl->mods[i] = NULL;
    }

    LL_APPEND(cn->controls, ctrl);
    return ctrl;
}

void ctrlnet_remove_control(control_network *cn, control *ctrl) {
    LL_DELETE(cn->controls, ctrl);
    free(ctrl);
}


void ctrlnet_slot_set_modulator(control_network *cn, control *ctrl, size_t slot, modulator *mod) {
    if (slot >= CONTROL_SLOTS) {
        fprintf(stderr, "ctrlnet_add_slot: invalid slot %d\n", slot);
        return;
    }

    ctrl->mods[slot] = mod;
}

void ctrlnet_slot_clear_modulator(control_network *cn, control *ctrl, size_t slot) {
    if (slot >= CONTROL_SLOTS) {
        fprintf(stderr, "ctrlnet_add_slot: invalid slot %d\n", slot);
        return;
    }

    ctrl->mods[slot] = NULL;
}

void ctrlnet_perform(control_network *cn, float *in, int n) {
    modulator *mod;
    control *ctrl;
    int i;

    // update all modulators
    LL_FOREACH(cn->modulators, mod) {
        mod->value = mod->perform_method(mod->data, in, n);
    }

    // update all controls
    LL_FOREACH(cn->controls, ctrl) {
        ctrl->value = ctrl->offset;

        // apply each modulator slot
        for (i = 0; i < CONTROL_SLOTS; i++) {
            if (ctrl->mods[i] != NULL) {
                ctrl->value += ctrl->amounts[i] * ctrl->mods[i]->value;
            }
        }
    }
}
