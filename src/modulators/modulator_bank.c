#include "modulators/modulator_bank.h"
#include <stdio.h>


modulator_bank *modulator_bank_new(goat_config *cfg) {
    modulator_bank *modbank = malloc(sizeof(modulator_bank));
    if (modbank == NULL) return NULL;

    char lfoname[16];
    for (int i = 0; i < MODBANK_NUM_LFOS; i++) {
        snprintf(lfoname, sizeof(lfoname), "lfo%d", i);

        modbank->lfos[i] = lfo_new(cfg, lfoname);
        if (modbank->lfos[i] == NULL) return NULL;
    }

    return modbank;
}

void modulator_bank_free(modulator_bank *modbank) {
    for (int i = 0; i < MODBANK_NUM_LFOS; i++) {
        lfo_free(modbank->lfos[i]);
    }

    free(modbank);
}
