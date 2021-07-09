#include "granular/granular.h"

#include "util/mem.h"


granular *granular_new(void) {
    granular *g = malloc(sizeof(granular));
    if (!g) return NULL;

    // create the circular buffer
    g->buffer = circbuf_new(1024);
    if (!g->buffer) return NULL;

    // add a single read tap
    if (!circbuf_readtap_add(g->buffer)) return NULL;
    g->buffer->readtaps->speed = 1.5f;

    return g;
}

void granular_free(granular *g) {
    circbuf_free(g->buffer);
    free(g);
}

void granular_perform(granular *g, float *in, float *out, int n) {
    /// @todo this is just placeholder / demo code
    circbuf_write_block(g->buffer, in, n);
    circbuf_read_block(g->buffer, g->buffer->readtaps, out, n);
}
