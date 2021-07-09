#include "goat.h"

#include <stdlib.h>


goat *goat_new(void) {
    goat *g = malloc(sizeof(goat));
    if (!g) return NULL;

    g->gran = granular_new();
    if (!g->gran) return NULL;

    return g;
}

void goat_free(goat *g) {
    granular_free(g->gran);
    free(g);
}

void goat_perform(goat *g, float *in, float *out, int n) {
    granular_perform(g->gran, in, out, n);
}
