#include "modulators/square/square.h"

#include <stdio.h>
#include <math.h>
#include "util/util.h"


square_modulator *square_new(goat_config *cfg, const char *name) {
    square_modulator *square = (square_modulator *) control_manager_modulator_add(cfg->mgr,
        name,
        (control_modulator_perform_method) square_perform,
        sizeof(square_modulator));
    char namebuf[32];

    square->cfg = cfg;

    snprintf(namebuf, sizeof(namebuf), "%s.frequency", name);
    square->frequency = control_manager_parameter_add(cfg->mgr,
        namebuf, 1.0f, 0.01f, 10.0f);

    return square;
}

void square_free(square_modulator *square) {
    control_manager_parameter_remove(square->cfg->mgr, square->frequency);

    // removing the modulator from the manager will automatically free the lfo subclass
    control_manager_modulator_remove(square->cfg->mgr, &square->super);
}

//to-do

void square_perform(square_modulator *square, __attribute__((unused)) float *in, int n) {

// phase?

    float p = square->phase;

    p += control_parameter_get_float(square->frequency) * (float) n / (float) square->cfg->sample_rate;
    p = p - ((int) p); // extract fractional part

    if((sinf(2 * M_PI * control_parameter_get_float(square->frequency) * (float) n + p)) > 0) square->super.value = -1;
    else square->super.value = 1;
    //square->super.value = sign(sinf(2 * M_PI * control_parameter_get_float(square->frequency) * (float) n + p)); // set the value

    square->phase = p;
}
