#include "modulators/lfo/lfo.h"

#include <stdio.h>
#include <math.h>
#include "util/util.h"


low_frequency_oscillator *lfo_new(goat_config *cfg, const char *name) {
    low_frequency_oscillator *lfo = (low_frequency_oscillator *) control_manager_modulator_add(cfg->mgr,
        name,
        (control_modulator_perform_method) lfo_perform,
        sizeof(low_frequency_oscillator));
    char namebuf[32];

    lfo->cfg = cfg;
    lfo->phase = 0.0f;

    snprintf(namebuf, sizeof(namebuf), "%s.frequency", name);
    lfo->frequency = control_manager_parameter_add(cfg->mgr,
        namebuf, 1.0f, 0.01f, 10.0f);

    return lfo;
}

void lfo_free(low_frequency_oscillator *lfo) {
    control_manager_parameter_remove(lfo->cfg->mgr, lfo->frequency);

    // removing the modulator from the manager will automatically free the lfo subclass
    control_manager_modulator_remove(lfo->cfg->mgr, &lfo->super);
}

void lfo_perform(low_frequency_oscillator *lfo, __attribute__((unused)) float *in, int n) {
    float p = lfo->phase;

    p += param(float, lfo->frequency) * (float) n / (float) lfo->cfg->sample_rate;
    p = p - ((int) p); // extract fractional part
    lfo->super.value = sinf(p * 2 * M_PI); // set the value

    lfo->phase = p;
}
