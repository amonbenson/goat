#include "control/modulator_bank.h"


modulator_bank *modulator_bank_new(control_manager *mgr) {
    modulator_bank *modbank = malloc(sizeof(modulator_bank));
    
    modbank->lfo1 = lfo_new(mgr, "lfo1");
    modbank->lfo2 = lfo_new(mgr, "lfo2");
    modbank->lfo3 = lfo_new(mgr, "lfo3");
    modbank->lfo4 = lfo_new(mgr, "lfo4");

    return modbank;
}

void modulator_bank_free(modulator_bank *modbank, control_manager *mgr) {
    lfo_free(modbank->lfo1, mgr);
    lfo_free(modbank->lfo2, mgr);
    lfo_free(modbank->lfo3, mgr);
    lfo_free(modbank->lfo4, mgr);

    free(modbank);
}
