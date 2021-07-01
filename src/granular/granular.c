#include "granular.h"

#include "mem.h"


granular *granular_new() {
    granular *g = malloc(sizeof(granular));
    if (!g) return NULL;

    // create the circular buffer
    g->buffer = circbuf_new(1024);
    if (!g->buffer) return NULL;

    // add one write and one read tap each
    if (!circbuf_writetap_add(g->buffer)) return NULL;
    if (!circbuf_readtap_add(g->buffer)) return NULL;

    // offset the read pointer to create a simple fixed delay (currently this has to be a multiple of the sample block size)
    g->buffer->readtaps->position = 1024 - 128;

    return g;
}

void granular_free(granular *g) {
    circbuf_free(g->buffer);
    free(g);
}

void granular_perform(granular *g, float *in, float *out, int n) {
    // TODO: do something useful
    circbuf_write_block(g->buffer, g->buffer->writetaps, in, n);
    circbuf_read_block(g->buffer, g->buffer->readtaps, out, n);
}
