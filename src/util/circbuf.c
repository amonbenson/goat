#include "util/circbuf.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"


circbuf_readtap *circbuf_readtap_new(void) {
    circbuf_readtap *tap = malloc(sizeof(circbuf_readtap));
    if (tap == NULL) return NULL;

    tap->position = 0.0f;
    tap->speed = 1.0f;
    tap->userdata = NULL;
    tap->next = NULL;

    return tap;
}

void circbuf_readtap_free(circbuf_readtap *tap) {
    if (tap->userdata != NULL) {
        fprintf(stderr, "circbuf_readtap_free: readtap contains userdata. Make sure to free the field and set it to NULL.\n");
    }

    free(tap);
}


circbuf *circbuf_new(size_t size) {
    if (!is_pwrtwo(size)) {
        fprintf(stderr, "circbuf_new: size must be a power of two %" PRI_SIZE_T "\n", size);
        return NULL;
    }

    circbuf *cb = malloc(sizeof(circbuf));
    if (!cb) return NULL;

    cb->data = malloc(sizeof(float) * size);

    cb->size = size;
    cb->writetap.position = 0;
    cb->writetap.userdata = NULL;
    cb->readtaps = NULL;

    return cb;
}

void circbuf_free(circbuf *cb) {
    // remove all read taps
    while (cb->readtaps) circbuf_readtap_remove(cb, cb->readtaps);

    if (cb->writetap.userdata != NULL) {
        fprintf(stderr, "circbuf_free: writetap contains userdata. Make sure to free the field and set it to NULL.\n");
    }

    // free the buffer itself
    free(cb->data);
    free(cb);
}

circbuf_readtap *circbuf_readtap_add(circbuf *cb) {
    circbuf_readtap *tap, *pred;

    tap = circbuf_readtap_new();
    if (tap == NULL) return NULL;

    // link the new tap to the end of the chain
    if (cb->readtaps == NULL) {
        cb->readtaps = tap;
    } else {
        for (pred = cb->readtaps; pred->next != NULL; pred = pred->next) continue;
        pred->next = tap;
    }

    return tap;
}

void circbuf_readtap_remove(circbuf *cb, circbuf_readtap *tap) {
    circbuf_readtap *pred;

    // find and unlink the tap
    if (cb->readtaps == tap) {
        cb->readtaps = NULL;
    } else {
        for (pred = cb->readtaps; pred != NULL && pred->next != tap; pred = pred->next);
        if (pred == NULL) {
            fprintf(stderr, "circbuf_readtap_remove: tap not found.\n");
            return;
        }

        pred->next = tap->next;
    }

    // free the tap
    circbuf_readtap_free(tap);
}

circbuf_readtap *circbuf_readtap_get(circbuf *cb, int index) {
    circbuf_readtap *tap;
    int i;

    // find the requested tap
    for (tap = cb->readtaps, i = 0; tap != NULL && i != index; tap = tap->next, i++);
    return tap;
}

void circbuf_write_block(circbuf *cb, float *src, size_t n) {
    size_t na, nb;

    if (n > cb->size) {
        fprintf(stderr, "circbuf_write_block: block size too large (%" PRI_SIZE_T " > %" PRI_SIZE_T ")",
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

float circbuf_read_interp(circbuf *cb, circbuf_readtap *tap) {
    /// @todo use some interpolation

    float sample = cb->data[(int) tap->position];

    tap->position += tap->speed;
    if (tap->position > cb->size) tap->position -= cb->size;

    return sample;
}

void circbuf_read_block(circbuf *cb, circbuf_readtap *tap, float *dst, size_t n) {
    if (n > cb->size) {
        fprintf(stderr, "circbuf_write_block: block size too large (%" PRI_SIZE_T " > %" PRI_SIZE_T ")",
            n, cb->size);
    }

    /// @todo when using interpolation, it is propably faster to store the previous value than to call @ref circbuf_read_interp each time
    while (n--) *dst++ = circbuf_read_interp(cb, tap);
}
