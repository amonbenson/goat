/**
 * @file vocaldetector.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stddef.h>
#include <math.h>
#include "util/util.h"


#define VD_PERIOD_MIN ((size_t) (44100 / 300)) /**< minimum period to detect */
#define VD_PERIOD_MAX ((size_t) (44100 / 100)) /**< maximum period to detect */

typedef unsigned long vd_block; /**< block type to be used by the bitwise correlation functions */
#define VD_BITS_PER_BLOCK (sizeof(vd_block) * 8) /**< number of bits in a single block */

#define VD_BUFFER_SIZE ((size_t) next_pwrtwo(VD_PERIOD_MAX * 2)) /**< size of the internal buffer. make sure the buffer will fit twice the maximum period */
#define VD_BLOCK_SIZE (VD_BUFFER_SIZE / VD_BITS_PER_BLOCK) /**< number of blocks in the internal buffer */

/**
 * @brief calculate the position of a zerocrossing between two samples using linear interpolation
 */
#define VD_SUBSAMPLE_POSITION(pos, s, s_last) (signbit(s) != signbit(s_last) \
    ? (float) (pos) - 1.0f + (float) (s_last) / ((float) (s_last) - (float) (s)) \
    : (float) (pos))

/**
 * @brief add two indices in a circular buffer
 */
#define VD_CIRC_ADD(x, y, buffersize) ((x) + (y) < (buffersize) ? (x) + (y) : ((x) + (y)) - (buffersize))

/**
 * @brief subtract two indices in a circular buffer
 */
#define VD_CIRC_SUB(x, y, buffersize) ((x) >= (y) ? (x) - (y) : ((x) + (buffersize)) - (y))

/**
 * @brief distance between two indices in a circular buffer
 */
#define VD_CIRC_DIST(x, y, buffersize) ((y) >= (x) ? (y) - (x) : (buffersize) - (x) + (y))


/**
 * @struct vocaldetector
 * @brief vocal detector structure
 */
typedef struct {
    size_t sample_rate; /**< sample rate of the audio data */

    float *buffer; /**< buffer for the incoming audio data */
    vd_block *bitstream; /**< generated bitstream for the pitch detection */

    size_t write_pos; /**< write position in the buffer */
    size_t marked_pos; /**< marked position in the buffer where the last period was detected */

    size_t sampled_period; /**< the last detected period rounded to samples */
    float period; /**< the last detected period with subsample accuracy */
    float frequency; /**< the last detected frequency */
    int voiced; /**< whether the last block was voiced or unvoiced audio data */
} vocaldetector;


/**
 * @memberof vocaldetector
 * @brief create a new vocal detector
 * 
 * @param sample_rate the sample rate of the audio data
 * @return vocaldetector* the new vocal detector
 */
vocaldetector *vd_new(size_t sample_rate);

/**
 * @memberof vocaldetector
 * @brief free a vocal detector
 * 
 * @param vd the vocal detector to free
 */
void vd_free(vocaldetector *vd);


/**
 * @memberof vocaldetector
 * @brief check if @a n is a valid blocksize to be processed by the detector
 * 
 * @param n the blocksize to check
 * @return int 1 if @a n is valid, 0 otherwise
 */
int is_valid_block_size(size_t n);

/**
 * @memberof vocaldetector
 * @brief debug print the current status
 * 
 * @param vd the vocal detector to print
 */
void vd_print(vocaldetector *vd);


/**
 * @memberof vocaldetector
 * @brief apply the bitstream autocorrelation (core of the vocaldetector) for a given number of blocks
 * 
 * @param vd the vocal detector to apply the autocorrelation on
 * @param a_pos the first index in the buffer
 * @param b_pos the second index in the buffer
 * @param n_blocks the number of blocks to process
 * @return float the autocorrelation value
 */
float vd_bitstream_correlate(vocaldetector *vd, size_t a_pos, size_t b_pos, size_t n_blocks);

/**
 * @memberof vocaldetector
 * @brief run the vocaldetector on a given block of audio data
 * 
 * @param vd the vocal detector
 * @param s the audio data to process
 * @param n the number of samples in @a s
 */
void vd_perform(vocaldetector *vd, float *s, size_t n);
