#include "control/modulators/lfo/lfo.h"

#include <stdio.h>
#include <math.h>
#include "util/util.h"


low_frequency_oscillator *lfo_new(control_manager *mgr, const char *name) {
    low_frequency_oscillator *lfo = (low_frequency_oscillator *) control_manager_modulator_add(mgr,
        name,
        (control_modulator_perform_method) lfo_perform,
        sizeof(low_frequency_oscillator));

    lfo->frequency = 100.0f; // TODO: currently unused

    return lfo;
}

void lfo_free(low_frequency_oscillator *lfo, control_manager *mgr) {
    // removing the modulator from the manager will automatically free the lfo subclass
    control_manager_modulator_remove(mgr, &lfo->super);
}

void lfo_perform(low_frequency_oscillator *lfo, __attribute__((unused)) float *in, __attribute__((unused)) int n) {
    float p = lfo->phase;
    lfo->super.value = sinf(p * 2 * M_PI);

    p += 0.003f; // TODO: make depended on frequency
    p -= ((int) p); // extract fractional part

    lfo->phase = p;
}
