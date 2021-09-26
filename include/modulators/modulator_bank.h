#pragma once
#include "goat_config.h"
#include "modulators/lfo/lfo.h"
#include "modulators/rand/rand_mod.h"
#include "modulators/vocaldetector/vocaldetector_mod.h"


#define MODBANK_NUM_LFOS 4
#define MODBANK_NUM_RANDS 4

/**
 * @struct modulator_bank 
 * @brief Set of pre-defined modulators.
 * 
 * The number of modulators is statically fixed mainly for performance reasons
 * and clarity to the user.
 */
typedef struct modulator_bank {
    goat_config *cfg; /**< global goat configuration */
    low_frequency_oscillator *lfos[MODBANK_NUM_LFOS]; /**< generic purpose LFOs */
    rand_mod *rms[MODBANK_NUM_RANDS]; /**< generic purpose random number generators */
    vocaldetector_mod *vodec; /**< vocal detector */
} modulator_bank;


/**
 * @memberof modulator_bank
 * @brief creates a new modulator bank
 * 
 * @param cfg global goat configuration
 * @return modulator_bank* a pointer to the new modulator bank or NULL if the allocation failed
 */
modulator_bank *modulator_bank_new(goat_config *cfg, vocaldetector *vd);

/**
 * @memberof modulator_bank
 * @brief frees an existing modulator bank
 * 
 * @param bank the modulator bank to be freed
 */
void modulator_bank_free(modulator_bank *bank);
