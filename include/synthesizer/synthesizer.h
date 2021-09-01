/**
 * @file grains.h
 * @author zeyu yang (zeyuyang42@163.com)
 * @brief ***********
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stdio.h>
#include <stddef.h>
#include "util/mem.h"
#include "util/util.h"

#include "m_pd.h" // add for post function, remove this after debuging

#include "params.h"
#include "util/circbuf.h"
#include "graintable/graintable.h"
#include "evelopbuf/evelopbuf.h"



typedef struct {
    float *data;
    int pos;
    int length;
    int repeat; /**< wether use this grain repeatly defalut 0 for no-repeat (remove after written into out stream)*/
} activategrain, *p_activategrain;


// *
//  * @struct synthesizer
//  * @brief circular buffer class
//  * 
//  * The circular buffer class contains a data array, the buffer size and references to the corresponding
//  * read and write taps.
typedef struct {

    p_activategrain *data; /**< The stored data itself */
    int length;        

} synthesizer;

/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
activategrain *activategrain_new(grain* gn, evelope* ep, int repeat);


/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
void activategrain_free(activategrain *ag);


/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
synthesizer *synthesizer_new(int length);


/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
void synthesizer_free(synthesizer *syn);


/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
void synthesizer_active_grain(synthesizer *syn, grain* gn, evelope* ep);


/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
void synthesizer_write_output(synthesizer *syn, float *out, int n);
