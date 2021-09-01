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
#include "util/mem.h"
#include "util/util.h"
#include "util/circbuf.h"

#include "m_pd.h" // add for post function, remove this after debuging

#include "params.h"

typedef struct {
    // basic features of a grain
    circbuf *buffer; /**< pointer to the buffer contains data to be sampled. could be delayline or internal wavetable(just imagination not in plan anyway) */
    int position;    /**< absolute start position of a grain at buffer*/
    int duration;    /**< length of a grain in sample,typically between */
    int  timeout;    /**< statue mark to tell if a grain still valid. this project involves a delayline rather than a static wavetable as source, the selected grain would be overwritten. */
    int  evelope;    /**< type of evelope, choose from 'Parabolic', 'Trapezoidal' and 'Raised Cosine Bell' */

    // advance features of a grain
    float frequency; /**< fundamental frequency of a grain. set to -1 if grain isn't assumed as monophonic or pitch detection is disabled*/
    float energy;    /**< simple framewise energy set to -1 if energy detection is disabled*/

}grain, *p_grain;


typedef struct{
    p_grain *data; /**< The stored data itself */
    int size; /**< The size of the buffer and its data array */

    int front;
    int rear;

} graintable; 


/**
 * @struct grain
 * @brief register a new grain with basic features
 * 
 * This function return a grain structure, with basic features that must be defined. 
 * Advance features can only be obtained by algorithms provided in analyzer module.
 * Basic features are enough to produce delay effect. Advance features give more flexible
 * and expressive control to each grain.
 */
grain *grain_new(circbuf *cb, int position, int duration, int  timeout, int evelope);


/**
 * @struct grain
 * @brief post all the features of a grain in pd-console for debug
 */
void grain_post_feature(grain *gn);


/**
 * @struct grain
 * @brief update timeout parameter at each dsp circle
 * 
 * Due to the nature of delayline, a selected grain in the buffer would be overwritten after a period of time.
 * Update timeout parameter at each dsp circle could reduce artifical effect caused by uncasual selection. 
 */
void grain_update_timeout(grain *gn, int n);


/**
 * @struct grain
 * @brief update advance features parameter after analyzing
 * 
 * Advance features can only derive after analyzing the selected section of audio in the buffer.
 * After analyzing procedure advance features will be updated in the grain structure.
 * basic features don't support update(for now)
 */
void grain_update_feature(grain *gn, float value, int feature);


/**
 * @struct grain
 * @brief free and existing a grain structure
 * 
 * free and existing a grain structure if it won't be used anymore or already overwritten.
 */
void grain_free(grain *gn);


//----------------------------------------------------------------------------------------------------------------------//


/**
 * @struct graintable
 * @brief create a graintable to store the information from sampled grains
 * 
 * Use queue as data structure of graintable. but not a strict queue, choose data from the middle could also be supported
 * 
 */
graintable *graintable_new(int size);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
void graintable_free(graintable *gt);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
void graintable_add_grain(graintable *gt, circbuf *cb, int grainsize, int evelope);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
grain *graintable_pop_grain(graintable *gt, grain *gn);


/**
 * @struct graintable
 * @brief 
 * 
 * check the information of a registed grain, not necessarily at the front
 */
void graintable_check_grain(graintable *gt, grain *gn, int position);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
int graintable_is_full(graintable *gt);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
int graintable_is_empty(graintable *gt);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * This class contains all information and provides functions regarding the core granular delay.
 * It does also contain functions to manipulate the delay (grain size, etc.)
 */
int graintable_get_len(graintable *gt);


/**
 * @struct graintable
 * @brief granular delay based around a circular buffer
 * 
 * update all grains' outtime parameter
 * 
 */
void graintable_update_timeout(graintable *gt, int n);


void graintable_print_all(graintable *gt);



