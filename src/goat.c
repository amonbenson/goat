#include "goat.h"

#include <stdlib.h>
#include "control/manager.h"


goat *goat_new(goat_config *config) {
    goat *g = malloc(sizeof(goat));
    if (!g) return NULL;

    memcpy(&g->cfg, config, sizeof(goat_config));

    g->cfg.mgr = control_manager_new();
    if (!g->cfg.mgr) return NULL;

    g->vd = vd_new(g->cfg.sample_rate);
    if (!g->vd) return NULL;

    g->modbank = modulator_bank_new(&g->cfg, g->vd);
    if (!g->modbank) return NULL;

    g->gran = granular_new();
    if (!g->gran) return NULL;

    g->schdur = scheduler_new(&g->cfg);
    if (!g->schdur) return NULL;   

    return g;
}

void goat_free(goat *g) {
    granular_free(g->gran);
    vd_free(g->vd);
    scheduler_free(g->schdur);
    modulator_bank_free(g->modbank);
    control_manager_free(g->cfg.mgr);
    free(g);
}

void goat_perform(goat *g, float *in, float *out, int n) {
    control_manager_perform(g->cfg.mgr, in, n);
    vd_perform(g->vd, in, n);
    scheduler_perform(g->schdur, n);
    granular_perform(g->gran, g->schdur, g->vd, in, out, n); // update the buffer and manipulate the DelayLine
}
