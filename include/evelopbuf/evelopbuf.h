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

typedef struct {
    float *data;
    int type;
    int length;
    int attacksamples;
    int releasesamples;
} evelope, *p_evelope;


/**
 * @struct circbuf
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
typedef struct {

    p_evelope *data; /**< The stored data itself */
    int size;        // maximum EVELOPBUFSIZE number of evelope with different type or length keep in the buffer, the early generated
    int front;
    int rear;

} evelopbuf;


/**
 * @struct evelope
 * @brief generate evelope under corresponding 
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelope *evelope_new(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples);


/**
 * @struct evelope
 * @brief generate evelope under corresponding 
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
void evelope_free(evelope* ep);


/**
 * @struct evelope
 * @brief generate evelope under corresponding 
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelope *evelope_gen_parabolic(evelope* ep, int type, int length, float amplitude);


/**
 * @struct evelope
 * @brief generate evelope under corresponding 
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelope *evelope_gen_trapezoidal(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples);


/**
 * @struct evelope
 * @brief generate evelope under corresponding 
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelope *evelope_gen_raised_cosine_bell(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples);


/**
 * @struct evelope
 * @brief generate evelope under corresponding 
 * 
 * useless all 1.0 evelop, only for debugging
 */
evelope *evelope_gen_no_evelope(evelope* ep, int type, int length, float amplitude);
//----------------------------------------------------------------------------------------------//


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelopbuf *evelopbuf_new(int size);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
void evelopbuf_free(evelopbuf *eb);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
void evelopbuf_add_evelope(evelopbuf *eb, int type, int length);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelope *evelopbuf_pop_evelope(evelopbuf *eb, evelope* ep);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
evelope *evelopbuf_check_evelope(evelopbuf *eb, int type, int length);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
int evelopbuf_is_full(evelopbuf *eb);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
int evelopbuf_is_empty(evelopbuf *eb);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
int evelopbuf_get_len(evelopbuf *eb);



