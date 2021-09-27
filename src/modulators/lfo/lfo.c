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
    
    snprintf(namebuf, sizeof(namebuf), "%s.curve", name);
    lfo->curve = control_manager_parameter_add(cfg->mgr,
        namebuf, LFO_CURVE_SINE, 0, LFO_NUM_CURVES - 1);

    return lfo;
}

void lfo_free(low_frequency_oscillator *lfo) {
    control_manager_parameter_remove(lfo->cfg->mgr, lfo->frequency);
    control_manager_parameter_remove(lfo->cfg->mgr, lfo->curve);

    // removing the modulator from the manager will automatically free the lfo subclass
    control_manager_modulator_remove(lfo->cfg->mgr, &lfo->super);
}

void lfo_perform(low_frequency_oscillator *lfo, __attribute__((unused)) float *in, int n) {
    float p = lfo->phase;
    float v;

    p += param(float, lfo->frequency) * (float) n / (float) lfo->cfg->sample_rate;
    p = p - ((int) p); // extract fractional part

    // use the selected curve to update the parameter
    switch (param(int, lfo->curve)) {
        case LFO_CURVE_SINE:
            v = sinf(p * 2.0f * M_PI);
            break;
        case LFO_CURVE_TRIANGLE:
            v = (p < 0.5f) ? (p * 4.0f - 1.0f) : (3.0f - p * 4.0f);
            break;
        case LFO_CURVE_SQUARE:
            v = (p < 0.5f) ? -1.0f : 1.0f;
            break;
        case LFO_CURVE_SAWTOOTH:
            v = p * 2.0f - 1.0f;
            break;
        case LFO_CURVE_SAWTOOTH_REVERSED:
            v = 1.0f - p * 2.0f;
            break;
        default:
            v = 0.0f;
            fprintf(stderr, "lfo_perform: unknown curve\n");
            break;
    }
    lfo->super.value = v;

    lfo->phase = p;
}
