/**
 * @file circbuf.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stddef.h>


/**
 * @def CIRCBUF_INRANGE(a, b, size)
 * @brief checks if x is in the range [a, b) where x, a and b are indices of a circular buffer
 */
#define CIRCBUF_INRANGE(a, b, x) ((a) <= (b) ? ((x) >= (a) && (x) <= (b)) : ((x) >= (a) || (x) <= (b)))

/**
 * @def CIRCBUF_DIST(a, b, size)
 * @brief calculate the distance between two indices in a circular buffer
 */
#define CIRCBUF_DIST(a, b, size) ((a) <= (b) ? (b) - (a) : (size) - (a) + (b))


/**
 * @struct circbuf_writetap
 * @related circbuf
 * @brief circular buffer write tap
 * 
 * This is the main write tap of a circular buffer. Each buffer has exactly one of these.
 */
typedef struct {
    size_t position; /**< buffer position of this tap */
} circbuf_writetap;

/**
 * @struct circbuf_readtap
 * @related circbuf
 * @brief circular buffer read tap
 * 
 * A buffer can have any number of read taps. They may have a different position and reading speed each.
 * Multiple read taps can be linked together as a single linked list.
 */
struct circbuf_readtap {
    float position; /**< interpolated buffer position of this tap */
    float speed; /**< speed at which the data is read */
};

/**
 * @cond Doxygen_Suppress
 * this suppression is kind of a workaround, because Doxygen doesn't seem to handle
 * the self reference of struct `circbuf_readtap *next` very well
 */
typedef struct circbuf_readtap circbuf_readtap;
/**
 * @endcond
 */

/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
typedef struct {
    float *data; /**< The stored data itself */
    size_t size; /**< The size of the buffer and its data array */
    size_t num_readtaps; /**< The number of read taps */

    circbuf_writetap writetap; /**< The write tap assigned to this buffer */
    circbuf_readtap readtaps[]; /**< A list of read taps or `NULL` if there are none */
} circbuf;


/**
 * @memberof circbuf
 * @brief create a new circular buffer of a specific @a size
 * 
 * @param size size of the circular buffer. This must be a power of two
 * @param num_readtaps number of read taps to create
 * @return circbuf* a reference to the allocated circular buffer or `NULL` if the allocation failed.
 */
circbuf *circbuf_new(size_t size, size_t num_readtaps);

/**
 * @memberof circbuf 
 * @brief free and existing circular buffer
 * 
 * This function will free all taps. Therefore, all userdata must be freed and set to NULL before
 * calling this function
 * @see circbuf_readtap_free
 * 
 * @param cb the buffer to be freed
 */
void circbuf_free(circbuf *cb);


/**
 * @memberof circbuf
 * @brief write a block of data into the buffer and update the write tap position
 * 
 * The function tries to copy all the data at once using `memcpy`.
 * If that is not possible, because the block cuts of at the end of the buffer, both halfs are
 * copied in two seperate steps.
 * 
 * the buffer's @ref circbuf_writetap.position is updated accordingly
 * 
 * @param cb the buffer to write data to
 * @param src the source to read the data from
 * @param n the number of samples to be written
 */
void circbuf_write_block(circbuf *cb, float *src, size_t n);

/**
 * @memberof circbuf
 * @brief read a single sample from the buffer at the specified @a tap
 * 
 * Because the readtap's position might be a float, multiple samples might be interpolated
 * After the sample is read, the position is moved forward according to @ref circbuf_readtap.speed
 * 
 * @param cb the buffer to read data from
 * @param tap the read tap index at which to read the data
 * @return float the sample that was read
 */
float circbuf_read_interp(circbuf *cb, size_t tap);

/**
 * @memberof circbuf
 * @brief read multiple samples
 * 
 * This function is equivalent to a repeated call of circbuf_read_interp(circbuf *, circbuf_readtap *),
 * but might yield faster results.
 * @see circbuf_read_interp
 * 
 * @param cb the buffer to read data from
 * @param tap the read tap index at which to read the data
 * @param dst the destination array to write the samples to
 * @param n the number of samples to be read
 */
void circbuf_read_block(circbuf *cb, size_t tap, float *dst, size_t n);
