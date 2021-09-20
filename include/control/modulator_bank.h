#pragma once
#include "control/modulators/lfo/lfo.h"


#define MODBANK_NUM_LFOS 4


typedef struct modulator_bank {
    low_frequency_oscillator *lfo1, *lfo2, *lfo3, *lfo4;
} modulator_bank;


modulator_bank *modulator_bank_new(control_manager *mgr);

void modulator_bank_free(modulator_bank *bank, control_manager *mgr);
