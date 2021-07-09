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
 * @struct circbuf_writetap
 * @related circbuf
 * @brief circular buffer write tap
 * 
 * This is the main write tap of a circular buffer. Each buffer has exactly one of these.
 */
typedef struct {
    size_t position; /**< buffer position of this tap */
    void *userdata; /**< optional pointer to a user data section. Before freeing, this must be set to `NULL` */
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
    void *userdata; /**< optional pointer to a user data section. Before freeing, this must be set to `NULL` */

    struct circbuf_readtap *next; /**< reference to the next read tap or `NULL`, if this is the last one in the chain */
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

    circbuf_writetap writetap; /**< The write tap assigned to this buffer */
    circbuf_readtap *readtaps; /**< A list of read taps or `NULL` if there are none */
} circbuf;


/**
 * @memberof circbuf_readtap
 * @brief create a new circbuf_readtap object
 * 
 * If you want to add a readtap to an exising circular buffer, use circbuf_readtap_add(circbuf *) instead.
 * @see circbuf_readtap_add
 * 
 * @return circbuf_readtap* a reference to the allocated circbuf_readtap or `NULL` if the allocation failed.
 */
circbuf_readtap *circbuf_readtap_new(void);

/**
 * @memberof circbuf_readtap
 * @brief free an existing circbuf_readtap object
 * 
 * @ref circbuf_readtap.userdata must be freed and set to `NULL` manually before calling this function.
 * If you want to remove a readtap from a an existing circular buffer, use circbuf_readtap_remove(circbuf *, circbuf_readtap *) instead.
 * @see circbuf_readtap_remove
 * 
 * @param tap the readtap to be freed
 */
void circbuf_readtap_free(circbuf_readtap *tap);


/**
 * @memberof circbuf
 * @brief create a new circular buffer of a specific @a size
 * 
 * @param size size of the circular buffer. This must be a power of two
 * @return circbuf* a reference to the allocated circular buffer or `NULL` if the allocation failed.
 */
circbuf *circbuf_new(size_t size);

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
 * @brief allocate and add a new read tap to a circular buffer
 * 
 * A new read tap will be allocated using circbuf_readtap_new and linked to the end of @ref circbuf.readtaps
 * 
 * @param cb the buffer to which the read tap is added
 * @return circbuf_readtap* the appended read tap or `NULL` if the creation failed.
 */
circbuf_readtap *circbuf_readtap_add(circbuf *cb);

/**
 * @memberof circbuf
 * @brief remove and free an existing read tap from a buffer
 * 
 * The readtap must be part of the @ref circbuf.readtaps list
 * Because the tap will be freed, its userdata must be cleared:
 * @see circbuf_readtap_free
 * 
 * @param cb the buffer from which the read tap is removed
 * @param tap the tap to be removed and freed
 */
void circbuf_readtap_remove(circbuf *cb, circbuf_readtap *tap);

/**
 * @memberof circbuf
 * @brief get the read tap at a specified index
 * 
 * because this function iterated through all taps each time it is called, it is recommended
 * to store a @ref circbuf_readtap* reference if multiple operations will be perfomed on that tap
 * 
 * @param cb the buffer from which to get the tap
 * @param index the index at which to find the tap
 * @return circbuf_readtap* a reference to the tap or `NULL` if the index was out of range
 */
circbuf_readtap *circbuf_readtap_get(circbuf *cb, int index);

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
 * @param tap the read tap at which to read the data. It must be a member of the buffer's @ref circbuf.readtaps
 * @return float the sample that was read
 */
float circbuf_read_interp(circbuf *cb, circbuf_readtap *tap);

/**
 * @memberof circbuf
 * @brief read multiple samples
 * 
 * This function is equivalent to a repeated call of circbuf_read_interp(circbuf *, circbuf_readtap *),
 * but might yield faster results.
 * @see circbuf_read_interp
 * 
 * @param cb the buffer to read data from
 * @param tap the read tap at which to read the data. It must be a member of the buffer's @ref circbuf.readtaps
 * @param dst the destination array to write the samples to
 * @param n the number of samples to be read
 */
void circbuf_read_block(circbuf *cb, circbuf_readtap *tap, float *dst, size_t n);
