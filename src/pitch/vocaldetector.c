#include "pitch/vocaldetector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "math.h"


vocaldetector *vd_new(size_t sample_rate) {
    printf("vocaldetector: buffersize = %" PRI_SIZE_T ", period_min = %" PRI_SIZE_T ", period_max = %" PRI_SIZE_T "\n",
        VD_BUFFER_SIZE, VD_PERIOD_MIN, VD_PERIOD_MAX);

    vocaldetector *vd = malloc(sizeof(vocaldetector));
    if (!vd) return NULL;

    vd->buffer = malloc(sizeof(float) * VD_BUFFER_SIZE);
    if (!vd->buffer) return NULL;
    for (size_t i = 0; i < VD_BUFFER_SIZE; i++) vd->buffer[i] = 0.0f;

    vd->bitstream = malloc(sizeof(vd_block) * VD_BLOCK_SIZE);
    if (!vd->bitstream) return NULL;
    for (size_t i = 0; i < VD_BLOCK_SIZE; i++) vd->bitstream[i] = 0;

    vd->sample_rate = sample_rate;

    vd->write_pos = 0;
    vd->marked_pos = 0;

    vd->period = 0.0f;
    vd->frequency = 0.0f;
    vd->voiced = 0;

    return vd;
}

void vd_free(vocaldetector *vd) {
    free(vd->buffer);
    free(vd);
}

int is_valid_block_size(size_t n) {
    return is_pwrtwo(n) && n >= VD_BITS_PER_BLOCK && n < VD_BUFFER_SIZE;
}

static void vd_block_print(vd_block block) {
    size_t j;

    for (j = 0; j < VD_BITS_PER_BLOCK; j++) {
        printf(block & ((vd_block) 1 << j) ? "#" : "-");
    }
}

void vd_print(vocaldetector *vd) {
    size_t i;

    // printf the buffer
    for (i = 0; i < VD_BLOCK_SIZE; i++) {
        vd_block_print(vd->bitstream[i]);
        printf(" ");
    }
    printf("\n");

    // print the position pointers
    for (i = 0; i < VD_BUFFER_SIZE; i++) {
        if (i == vd->write_pos) printf("W");
        else if (i == vd->marked_pos) printf("|");
        else if (i == VD_CIRC_ADD(vd->marked_pos, VD_PERIOD_MIN, VD_BUFFER_SIZE)) printf("m");
        else if (i == VD_CIRC_ADD(vd->marked_pos, VD_PERIOD_MAX, VD_BUFFER_SIZE)) printf("M");
        else printf(" ");

        if ((i + 1) % VD_BITS_PER_BLOCK == 0) printf(" ");
    }
    printf("\n\n");
}

float vd_bitstream_correlate(vocaldetector *vd, size_t a_pos, size_t b_pos, size_t n) {
    vd_block block_a, block_b, block_corr, block_endmask, *bs;
    size_t n_blocks, i, a, a_next, a_shift, b, b_next, b_shift, sum;
    
    bs = vd->bitstream;

    n_blocks = (n - 1) / VD_BITS_PER_BLOCK + 1; // ceil
    block_endmask = ((vd_block) 1 << (n % VD_BITS_PER_BLOCK)) - 1;
    if (block_endmask == 0) block_endmask = (vd_block) -1; // if n is a multiple of the block size, set all mask bits

    a = a_pos / VD_BITS_PER_BLOCK;
    b = b_pos / VD_BITS_PER_BLOCK;
    a_shift = a_pos % VD_BITS_PER_BLOCK;
    b_shift = b_pos % VD_BITS_PER_BLOCK;
    sum = 0;

    for (i = 0; i < n_blocks; i++, a = a_next, b = b_next) {
        a_next = VD_CIRC_ADD(a, 1, VD_BLOCK_SIZE);
        b_next = VD_CIRC_ADD(b, 1, VD_BLOCK_SIZE);

        block_a = bs[a] >> a_shift | bs[a_next] << (VD_BITS_PER_BLOCK - a_shift);
        block_b = bs[b] >> b_shift | bs[b_next] << (VD_BITS_PER_BLOCK - b_shift);
        
        // correlate using bitwise XNOR
        block_corr = ~(block_a ^ block_b);
        if (i == n_blocks - 1) block_corr &= block_endmask;
        sum += util_popcount(block_corr);
    }

    return (float) sum / (float) n;
}

static void vd_process_signal(vocaldetector *vd, float *s, size_t n) {
    size_t i, j, write_pos_blocks, n_blocks;
    vd_block block;

    n_blocks = n / VD_BITS_PER_BLOCK;
    write_pos_blocks = vd->write_pos / VD_BITS_PER_BLOCK;

    // buffer the incoming signal
    memcpy(vd->buffer + vd->write_pos, s, n * sizeof(float));

    // construct the bitstream
    for (i = 0; i < n_blocks; i++) {

        // set each bit where the input signal is positive
        block = 0;
        for (j = 0; j < VD_BITS_PER_BLOCK; j++, s++) {
            if (*s > 0.0f) block |= (vd_block) 1 << j;
        }

        vd->bitstream[write_pos_blocks + i] = block;
        //vd_block_print(&block);
    }

    // move the write position
    vd->write_pos = VD_CIRC_ADD(vd->write_pos, n, VD_BUFFER_SIZE);
}

static int is_better_period(__attribute__((unused)) vocaldetector *vd, __attribute__((unused)) float period, float correlation, float best_correlation) {
    return correlation * 0.8f > best_correlation;
}

static void vd_detect_period(vocaldetector *vd) {
    size_t a_pos, b_pos;
    size_t sampled_period, best_sampled_period;
    float s, s_last, a_sub, b_sub, period, best_period, correlation, best_correlation;

    a_pos = vd->marked_pos;
    s_last = vd->buffer[VD_CIRC_ADD(a_pos, VD_PERIOD_MIN - 1, VD_BUFFER_SIZE)];

    a_sub = VD_SUBSAMPLE_POSITION(a_pos,
        vd->buffer[a_pos],
        vd->buffer[VD_CIRC_SUB(a_pos, 1, VD_BUFFER_SIZE)]);

    sampled_period = 0;
    period = 0.0f;
    correlation = 0.0f;
    
    best_sampled_period = 0;
    best_period = -1.0f;
    best_correlation = 0.0f;

    for (sampled_period = VD_PERIOD_MIN, s = 0.0f; sampled_period <= VD_PERIOD_MAX; sampled_period++, s_last = s) {
        b_pos = VD_CIRC_ADD(a_pos, sampled_period, VD_BUFFER_SIZE);

        // search for a rising edge
        s = vd->buffer[b_pos];
        if (!(s > 0.0f && s_last <= 0.0f)) continue;

        // get the subsamples period
        b_sub = VD_SUBSAMPLE_POSITION(b_pos, s, s_last);
        period = VD_CIRC_SUB(b_sub, a_sub, (float) VD_BUFFER_SIZE);

        // run the correlation
        correlation = vd_bitstream_correlate(vd, a_pos, b_pos, period * 2);
        //printf("%04zu..%04zu(%f) ", a_pos, b_pos, correlation);

        if (best_sampled_period == 0 || is_better_period(vd, period, correlation, best_correlation)) {
            best_sampled_period = sampled_period;
            best_period = period;
            best_correlation = correlation;
        }
    }
    //printf("\n");

    vd->sampled_period = best_sampled_period;
    vd->period = best_period;

    if (vd->period < VD_PERIOD_MIN || vd->period > VD_PERIOD_MAX) {
        vd->period = -1.0f;
        vd->frequency = -1.0f;
        vd->voiced = 0;
    } else {
        vd->frequency = vd->sample_rate / vd->period;
        vd->voiced = 1;
    }
}

void vd_perform(vocaldetector *vd, float *s, size_t n) {
    if (!is_valid_block_size(n)) {
        fprintf(stderr, "vocaldetector: invalid block size: %" PRI_SIZE_T "\n", n);
        return;
    };

    vd_process_signal(vd, s, n);

    // invoke the detector when we've read enough samples
    while (VD_CIRC_DIST(vd->marked_pos, vd->write_pos, VD_BUFFER_SIZE) >= VD_PERIOD_MAX) {
        vd_detect_period(vd);

        // update the marked position
        vd->marked_pos = VD_CIRC_ADD(vd->marked_pos,
            vd->sampled_period == 0 ? VD_PERIOD_MAX : vd->sampled_period,
            VD_BUFFER_SIZE);
    }
}
