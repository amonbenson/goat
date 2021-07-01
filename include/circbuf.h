#pragma once

#include <stddef.h>


typedef struct _circbuf_tap {
    size_t position;
    void *userdata;

    struct _circbuf_tap *next;
} circbuf_tap;

typedef struct {
    float *data;
    size_t size;

    circbuf_tap *writetaps;
    circbuf_tap *readtaps;
} circbuf;


circbuf_tap *circbuf_tap_new();

void circbuf_tap_free(circbuf_tap *tap);


circbuf *circbuf_new(size_t size);

void circbuf_free(circbuf *cb);

circbuf_tap *circbuf_writetap_add(circbuf *cb);
void circbuf_writetap_remove(circbuf *cb, circbuf_tap *tap);
circbuf_tap *circbuf_writetap_get(circbuf *cb, int index);

circbuf_tap *circbuf_readtap_add(circbuf *cb);
void circbuf_readtap_remove(circbuf *cb, circbuf_tap *tap);
circbuf_tap *circbuf_readtap_get(circbuf *cb, int index);

void circbuf_write_block(circbuf *cb, circbuf_tap *tap, float *src, int n);
void circbuf_read_block(circbuf *cb, circbuf_tap *tap, float *dst, int n);
