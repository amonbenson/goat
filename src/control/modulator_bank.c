#include "control/modulator_bank.h"


modulator_bank *modulator_bank_new(control_manager *mgr) {
    modulator_bank *modbank = malloc(sizeof(modulator_bank));

    char lfoname[] = "lfoN";
    for (int i = 0; i < MODBANK_NUM_LFOS; i++) {
        lfoname[3] = '1' + i;
        modbank->lfos[i] = lfo_new(mgr, lfoname);
    }

    return modbank;
}

void modulator_bank_free(modulator_bank *modbank, control_manager *mgr) {
    for (int i = 0; i < MODBANK_NUM_LFOS; i++) {
        lfo_free(modbank->lfos[i], mgr);
    }

    free(modbank);
}
