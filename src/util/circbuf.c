#include "circbuf.h"

#include <stdio.h>
#include "mem.h"
#include "util.h"


circbuf_tap *circbuf_tap_new() {
    circbuf_tap *tap = malloc(sizeof(circbuf_tap));
    if (tap == NULL) return NULL;

    tap->next = NULL;
    tap->position = 0;
    tap->userdata = NULL;

    return tap;
}

void circbuf_tap_free(circbuf_tap *tap) {
    if (tap->userdata != NULL) {
        fprintf(stderr, "circbuf_tap_free: tap still contains userdata. Make sure to free the field and set it to NULL.\n");
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
    cb->writetaps = NULL;
    cb->readtaps = NULL;

    return cb;
}

void circbuf_free(circbuf *cb) {
    // remove all taps
    while (cb->writetaps) circbuf_writetap_remove(cb, cb->writetaps);
    while (cb->readtaps) circbuf_readtap_remove(cb, cb->readtaps);

    // free the buffer itself
    free(cb->data);
    free(cb);
}

circbuf_tap *circbuf_writetap_add(circbuf *cb) {
    circbuf_tap *tap, *pred;

    tap = circbuf_tap_new();
    if (tap == NULL) return NULL;

    // link the new tap to the end of the chain
    if (cb->writetaps == NULL) {
        cb->writetaps = tap;
    } else {
        for (pred = cb->writetaps; pred->next != NULL; pred = pred->next);
        pred->next = tap;
    }

    return tap;
}

void circbuf_writetap_remove(circbuf *cb, circbuf_tap *tap) {
    circbuf_tap *pred;

    // find and unlink the tap
    if (cb->writetaps == tap) {
        cb->writetaps = NULL;
    } else {
        for (pred = cb->writetaps; pred != NULL && pred->next != tap; pred = pred->next);
        if (pred == NULL) {
            fprintf(stderr, "circbuf_writetap_remove: tap not found.\n");
            return;
        }

        pred->next = tap->next;
    }

    // free the tap
    circbuf_tap_free(tap);
}

circbuf_tap *circbuf_writetap_get(circbuf *cb, int index) {
    circbuf_tap *tap;
    int i;

    // find the requested tap
    for (tap = cb->writetaps, i = 0; tap != NULL && i != index; tap = tap->next, i++);
    return tap;
}

circbuf_tap *circbuf_readtap_add(circbuf *cb) {
    circbuf_tap *tap, *pred;

    tap = circbuf_tap_new();
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

void circbuf_readtap_remove(circbuf *cb, circbuf_tap *tap) {
    circbuf_tap *pred;

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
    circbuf_tap_free(tap);
}

circbuf_tap *circbuf_readtap_get(circbuf *cb, int index) {
    circbuf_tap *tap;
    int i;

    // find the requested tap
    for (tap = cb->readtaps, i = 0; tap != NULL && i != index; tap = tap->next, i++);
    return tap;
}

void circbuf_write_block(circbuf *cb, circbuf_tap *tap, float *src, int n) {
    // TODO: account for cases where the pointer needs to wrap around midway through a write operation

    memcpy(&cb->data[tap->position], src, sizeof(float) * n);
    tap->position = fastmod(tap->position + n, cb->size);
}

void circbuf_read_block(circbuf *cb, circbuf_tap *tap, float *dst, int n) {
    // TODO: account for cases where the pointer needs to wrap around midway through a read operation

    memcpy(dst, &cb->data[tap->position], sizeof(float) * n);
    tap->position = fastmod(tap->position + n, cb->size);
}
