#pragma once

#include <stddef.h>


typedef struct _circbuf_writetap {
    size_t position;
    void *userdata;
} circbuf_writetap;

typedef struct _circbuf_readtap {
    float position;
    float speed;
    void *userdata;

    struct _circbuf_readtap *next;
} circbuf_readtap;

typedef struct {
    float *data;
    size_t size;

    circbuf_writetap writetap;
    circbuf_readtap *readtaps;
} circbuf;


circbuf_readtap *circbuf_readtap_new();

void circbuf_readtap_free(circbuf_readtap *tap);


circbuf *circbuf_new(size_t size);

void circbuf_free(circbuf *cb);

circbuf_readtap *circbuf_readtap_add(circbuf *cb);
void circbuf_readtap_remove(circbuf *cb, circbuf_readtap *tap);
circbuf_readtap *circbuf_readtap_get(circbuf *cb, int index);

void circbuf_write_block(circbuf *cb, float *src, size_t n);

float circbuf_read_interp(circbuf *cb, circbuf_readtap *tap);
void circbuf_read_block(circbuf *cb, circbuf_readtap *tap, float *dst, size_t n);
