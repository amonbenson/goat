#include "goat.h"

#include <stdlib.h>
// #include <unistd.h>

goat *goat_new(void) {
    goat *g = malloc(sizeof(goat));
    if (!g) return NULL;

    g->gran = granular_new();
    if (!g->gran) return NULL;

    g->schdur = scheduler_new();
    if (!g->schdur) return NULL;   

    return g;
}

void goat_free(goat *g) {
    granular_free(g->gran);
    scheduler_free(g->schdur);
    free(g);
}

void goat_perform(goat *g, float *in, float *out, int n) {
    scheduler_perform(g->schdur);
    granular_perform(g->gran, g->schdur, in, out, n); // update the buffer and manipulate the DelayLine
}


