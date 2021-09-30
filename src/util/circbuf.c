#include "util/circbuf.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"


circbuf *circbuf_new(size_t size, size_t num_readtaps) {
    if (!is_pwrtwo(size)) {
        fprintf(stderr, "circbuf_new: size must be a power of two %" PRI_SIZE_T "\n", size);
        return NULL;
    }

    circbuf *cb = malloc(sizeof(circbuf) + sizeof(circbuf_readtap) * num_readtaps);
    if (cb == NULL) return NULL;

    cb->data = malloc(sizeof(float) * size);
    if (cb->data == NULL) return NULL; 

    cb->size = size;
    cb->num_readtaps = num_readtaps;

    // initialize the writetap
    cb->writetap.position = 0;

    // initialize each readtap
    for (size_t i = 0; i < num_readtaps; i++) {
        cb->readtaps[i].position = 0.0f;
        cb->readtaps[i].speed = 1.0f;
    }

    return cb;
}

void circbuf_free(circbuf *cb) {
    free(cb->data);
    free(cb);
}

void circbuf_write_block(circbuf *cb, float *src, size_t n) {
    size_t na, nb;

    if (n > cb->size) {
        fprintf(stderr, "circbuf_write_block: block size too large (%" PRI_SIZE_T " > %" PRI_SIZE_T ")\n",
            n, cb->size);
    }

    if (cb->writetap.position + n <= cb->size) {
        // simple copy
        memcpy(&cb->data[cb->writetap.position], src, sizeof(float) * n);
    } else {
        // target destination wraps around: we need to copy in two steps
        na = cb->size - cb->writetap.position;
        nb = cb->writetap.position + n - cb->size;

        memcpy(&cb->data[cb->writetap.position], src, sizeof(float) * na);
        memcpy(cb->data, src + na, sizeof(float) * nb);
    }

    cb->writetap.position += n;
    if (cb->writetap.position >= cb->size) cb->writetap.position -= cb->size;
}

float circbuf_read_interp(circbuf *cb, size_t tap) {
    if (tap >= cb->num_readtaps) {
        fprintf(stderr, "circbuf_read_interp: tap index out of bounds (%" PRI_SIZE_T " >= %" PRI_SIZE_T ")\n",
            tap, cb->num_readtaps);
    }

    /// @todo use some interpolation
    circbuf_readtap *t = &cb->readtaps[tap];
    float sample = cb->data[(int) t->position];

    t->position += t->speed;

    if (t->position < 0.0f) t->position += cb->size;
    if (t->position >= cb->size) t->position -= cb->size;

    return sample;
}

void circbuf_read_block(circbuf *cb, size_t tap, float *dst, size_t n) {
    if (n > cb->size) {
        fprintf(stderr, "circbuf_write_block: block size too large (%" PRI_SIZE_T " > %" PRI_SIZE_T ")\n",
            n, cb->size);
    }

    /// @todo when using interpolation, it is propably faster to store the previous value than to call @ref circbuf_read_interp each time
    while (n--) *dst++ = circbuf_read_interp(cb, tap);
}
