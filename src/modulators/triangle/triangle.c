#include "modulators/triangle/triangle.h"

#include <stdio.h>
#include <math.h>
#include "util/util.h"


triangle_modulator *triangle_new(goat_config *cfg, const char *name) {
    triangle_modulator *triangle = (triangle_modulator *) control_manager_modulator_add(cfg->mgr,
        name,
        (control_modulator_perform_method) triangle_perform,
        sizeof(triangle_modulator));
    char namebuf[32];

    triangle->cfg = cfg;

    snprintf(namebuf, sizeof(namebuf), "%s.frequency", name);
    triangle->frequency = control_manager_parameter_add(cfg->mgr,
        namebuf, 1.0f, 0.01f, 10.0f);

    return triangle;
}

void triangle_free(triangle_modulator *triangle) {
    control_manager_parameter_remove(triangle->cfg->mgr, triangle->frequency);

    // removing the modulator from the manager will automatically free the lfo subclass
    control_manager_modulator_remove(triangle->cfg->mgr, &triangle->super);
}

//to-do

void triangle_perform(triangle_modulator *triangle, __attribute__((unused)) float *in, int n) {
    float p = triangle->phase;

    float m = (float) triangle->cfg->sample_rate / control_parameter_get_float(triangle->frequency);

    p += control_parameter_get_float(triangle->frequency) * (float) n / (float) triangle->cfg->sample_rate;
    p = p - ((int) p); // extract fractional part

    if(fmod(m,p) == 0) triangle->super.value = 0;
    triangle->super.value = m*p; // set the value

    triangle->phase = p;
}
