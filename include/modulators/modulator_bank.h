#pragma once
#include "modulators/lfo/lfo.h"


#define MODBANK_NUM_LFOS 4

/**
 * @struct modulator_bank 
 * @brief Set of pre-defined modulators.
 * 
 * The number of modulators is statically fixed mainly for performance reasons
 * and clarity to the user.
 */
typedef struct modulator_bank {
    low_frequency_oscillator *lfos[MODBANK_NUM_LFOS]; /**< generic purpose LFOs */
} modulator_bank;


/**
 * @memberof modulator_bank
 * @brief creates a new modulator bank
 * 
 * @param mgr the control manager to add modulators to
 * @return modulator_bank* a pointer to the new modulator bank or NULL if the allocation failed
 */
modulator_bank *modulator_bank_new(control_manager *mgr);

/**
 * @memberof modulator_bank
 * @brief frees an existing modulator bank
 * 
 * @param bank the modulator bank to be freed
 * @param mgr the control manager to remove the modulators from
 */
void modulator_bank_free(modulator_bank *bank, control_manager *mgr);
