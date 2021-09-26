#pragma once

#include <stddef.h>
#include <math.h>
#include "util/util.h"


#define VD_PERIOD_MIN ((size_t) (44100 / 1000))
#define VD_PERIOD_MAX ((size_t) (44100 / 50))

typedef unsigned long vd_block;
#define VD_BITS_PER_BLOCK (sizeof(vd_block) * 8)

#define VD_BUFFER_SIZE ((size_t) next_pwrtwo(VD_PERIOD_MAX * 2)) // make sure the buffer will fit twice the maximum period
#define VD_BLOCK_SIZE (VD_BUFFER_SIZE / VD_BITS_PER_BLOCK)

#define VD_SUBSAMPLE_POSITION(pos, s, s_last) (signbit(s) != signbit(s_last) \
    ? (float) (pos) - 1.0f + (float) (s_last) / ((float) (s_last) - (float) (s)) \
    : (float) (pos))

#define VD_CIRC_ADD(x, y, buffersize) ((x) + (y) < (buffersize) ? (x) + (y) : ((x) + (y)) - (buffersize))
#define VD_CIRC_SUB(x, y, buffersize) ((x) >= (y) ? (x) - (y) : ((x) + (buffersize)) - (y))
#define VD_CIRC_DIST(x, y, buffersize) ((y) >= (x) ? (y) - (x) : (buffersize) - (x) + (y))


typedef struct _vocaldetector {
    float *buffer;
    vd_block *bitstream;

    size_t write_pos;
    size_t marked_pos;

    size_t sampled_period;
    float period;
} vocaldetector;


vocaldetector *vd_new();
void vd_free(vocaldetector *vd);

int is_valid_block_size(size_t n);
void vd_print(vocaldetector *vd);

float vd_bitstream_correlate(vocaldetector *vd, size_t a_pos, size_t b_pos, size_t n_blocks);
void vd_perform(vocaldetector *vd, float *s, size_t n);
