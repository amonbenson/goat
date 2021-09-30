#include <stdio.h>
#include "modulators/vocaldetector/vocaldetector_mod.h"
#include "control/manager.h"


vocaldetector_mod *vdmod_new(goat_config *cfg, vocaldetector *vd, const char *name) {
    vocaldetector_mod *vdmod = (vocaldetector_mod *) control_manager_modulator_add(cfg->mgr,
        name,
        (control_modulator_perform_method) vdmod_perform,
        sizeof(vocaldetector_mod));
    char namebuf[32];

    vdmod->cfg = cfg;
    vdmod->vd = vd;

    snprintf(namebuf, sizeof(namebuf), "%s.factor", name);
    vdmod->factor = control_manager_parameter_add(cfg->mgr,
        namebuf, 1.0f, -1.0f, 1.0f);

    return vdmod;
}

void vdmod_free(vocaldetector_mod *vdmod) {
    control_manager_parameter_remove(vdmod->cfg->mgr, vdmod->factor);

    // removing the modulator from the manager will automatically free the vd_mod subclass
    control_manager_modulator_remove(vdmod->cfg->mgr, &vdmod->super);
}

void vdmod_perform(vocaldetector_mod *vdmod, __attribute__((unused)) float *in, __attribute__((unused)) int n) {
    vdmod->super.value = vdmod->vd->frequency * param(float, vdmod->factor);
}
