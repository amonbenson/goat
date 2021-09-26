#include "modulators/modulator_bank.h"
#include <stdio.h>


modulator_bank *modulator_bank_new(goat_config *cfg, vocaldetector *vd) {
    modulator_bank *modbank = malloc(sizeof(modulator_bank));
    if (modbank == NULL) return NULL;
    char namebuf[50];

    for (int i = 0; i < MODBANK_NUM_LFOS; i++) {
        snprintf(namebuf, sizeof(namebuf), "lfo%d", i + 1);

        modbank->lfos[i] = lfo_new(cfg, namebuf);
        if (modbank->lfos[i] == NULL) return NULL;
    }

    for (int i = 0; i < MODBANK_NUM_RANDS; i++) {
        snprintf(namebuf, sizeof(namebuf), "rand%d", i + 1);

        modbank->rms[i] = rand_mod_new(cfg, namebuf);
        if (modbank->rms[i] == NULL) return NULL;
    }

    modbank->vodec = vdmod_new(cfg, vd, "vodec");

    return modbank;
}

void modulator_bank_free(modulator_bank *modbank) {
    for (int i = 0; i < MODBANK_NUM_LFOS; i++) {
        lfo_free(modbank->lfos[i]);
    }

    for (int i = 0; i < MODBANK_NUM_RANDS; i++) {
        rand_mod_free(modbank->rms[i]);
    }

    vdmod_free(modbank->vodec);

    free(modbank);
}
