#include "modulators/saw/saw.h"

#include <stdio.h>
#include <math.h>
#include "util/util.h"


sawtooth_modulator *saw_new(goat_config *cfg, const char *name) {
    sawtooth_modulator *saw = (sawtooth_modulator *) control_manager_modulator_add(cfg->mgr,
        name,
        (control_modulator_perform_method) saw_perform,
        sizeof(sawtooth_modulator));
    char namebuf[32];

    saw->cfg = cfg;

    snprintf(namebuf, sizeof(namebuf), "%s.frequency", name);

// check values

    saw->frequency = control_manager_parameter_add(cfg->mgr,
        namebuf, 1.0f, 0.01f, 10.0f);

    return saw;
}

void saw_free(sawtooth_modulator *saw) {
    control_manager_parameter_remove(saw->cfg->mgr, saw->frequency);

    // removing the modulator from the manager will automatically free the saw subclass
    control_manager_modulator_remove(saw->cfg->mgr, &saw->super);
}

//to-do 

void saw_perform(sawtooth_modulator *saw, __attribute__((unused)) float *in, int n) {
    float p = saw->phase;

// update to saw algorithm


    p += control_parameter_get_float(saw->frequency) * (float) n / (float) saw->cfg->sample_rate;
    p = p - ((int) p); // extract fractional part
    float full_period = 1/control_parameter_get_float(saw->frequency);
    float local_time =fmod(p,full_period);


    saw->super.value = (local_time/full_period)*2 -1.0;

    saw->phase = p;
}