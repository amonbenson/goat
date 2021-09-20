/**
 * @file graintable.h
 * @author zeyu yang (zeyuuyang42@gmail.com)
 * @brief ***********
 * @version 0.2
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

/**
 * @struct grain
 * @brief store information of a grain 
 * 
 * This struct contains all the information needed to describe, sample and generate a grain
 */
typedef struct {
    // basic features of a grain
    circbuf *buffer; /**< pointer to the buffer contains data to be sampled */
    int position;    /**< absolute start position of a grain at buffer */
    int duration;    /**< length of a grain in sample */
    int  timeout;    /**< statue mark to tell if a grain still valid. suit for DelayLine grain source */
    int  evelope;    /**< type of evelope to be applied on this grain */

    // advance features of a grain
    float frequency; /**< fundamental frequency of a grain. set to -1 if grain isn't assumed as monophonic or pitch detection is disabled*/
    float energy;    /**< simple framewise energy set to -1 if energy detection is disabled*/

}grain, *p_grain;


/**
 * @struct graintable
 * @brief data structure stores grain objects
 */
typedef struct{
    p_grain *data;   /**< The stored data itself */
    int size;        /**< The buffer size */
    int front;       /**< The indicator to the front position */
    int rear;        /**< The indicator to the rear position */

} graintable; 


/**
 * @memberof grain
 * @brief generate grain 
 * 
 * This method creates a new grain object
 * 
 * @param cb the circle buffer object as the source of grains
 * @param position absolute start position of a grain at buffer
 * @param duration length of a grain in sample
 * @param timeout statue mark to tell if a grain still valid
 * @param evelope type of evelope to be applied on this grain
 * 
 * @return grain* a reference to the grain object or `NULL` if failed
 */
grain *grain_new(circbuf *cb, int position, int duration, int  timeout, int evelope);

/**
 * @memberof grain
 * @brief frees a grain structure
 * 
 * This method frees a grain object
 * 
 * @param gn the grain object to be freed
 */
void grain_free(grain *gn);

/**
 * @memberof grain
 * @brief post all the features of a grain in pd-console for debugging
 * 
 * @param gn the grain object to be posted
 */
void grain_post_feature(grain *gn);

/**
 * @memberof grain
 * @brief update timeout parameter at each dsp circle
 * 
 * Due to the nature of delayline, a selected grain in the buffer would be overwritten after a period of time
 * Update timeout parameter at each dsp circle could reduce artifical effect caused by uncasual selection
 * 
 * @param gn the grain object to be updated
 * @param n number of new samples stored in audio buffer
 */
void grain_update_timeout(grain *gn, int n);

/**
 * @memberof grain
 * @brief update advance features parameter after analyzing
 * 
 * Advance features can only derive after analyzing the selected section of audio in the buffer
 * After analyzing procedure advance features will be updated in the grain structure
 * basic features don't support update(for now)
 * 
 * @param gn the grain object to be updated
 * @param value value of feature to be updated
 * @param feature type of feature to be updated
 */
void grain_update_feature(grain *gn, float value, int feature);


/**
 * @memberof graintable
 * @brief create a graintable to store the information from sampled grains
 * 
 * This method creates a graintable object
 * Graintable stores all grains object using queue as data structure
 * Not a strict queue, because check grains from middle still supported
 * 
 * @param size the size of graintable
 * 
 * @return graintable* a reference to the graintable object or `NULL` if failed
 */
graintable *graintable_new(int size);

/**
 * @memberof graintable
 * @brief frees a graintable object
 * 
 * This method frees a graintable object
 * 
 * @param gt the graintable object to be freed
 */
void graintable_free(graintable *gt);

/**
 * @memberof graintable
 * @brief adds grain into graintable
 * 
 * This method adds a new grain object into graintable
 * The new grain is sampled according to the circle buffer's 
 * ReadTap position and parameters from scheduler
 * 
 * @param gt the graintable object to store the new grain
 * @param cb the circle buffer to sample grain
 * @param grainsize the size of grain
 * @param evelope the tyoe of evelope of grain
 */
void graintable_add_grain(graintable *gt, circbuf *cb, int grainsize, int evelope); 

/**
 * @memberof graintable
 * @brief pops the front grain out of graintable
 * 
 * This method pop the front grain object out of graintable when the graintable is not empty
 * 
 * @param gt graintable object where grain object will be popped
 * @param gn grain object to store popped grain object
 * 
 * @return grain* a reference to the grain object or `NULL` if failed
 */
grain *graintable_pop_grain(graintable *gt, grain *gn);

/**
 * @memberof graintable
 * @brief checks the information of a registed grain
 * 
 * This method check the information of a registed grain which is not necessarily at the front
 * The diffenent from graintable_pop_grain is that whether the grain will be freed 
 */
void graintable_check_grain(graintable *gt, grain *gn, int position);

/**
 * @memberof graintable
 * @brief checks if the evelopbuf is full
 * 
 * This method checks if the evelopbuf is full
 * 
 * @param gt evelopbuf object to be checked
 * 
 * @return 0 or 1 indicates N or Y
 */
int graintable_is_full(graintable *gt);

/**
 * @memberof graintable
 * @brief checks if the graintable is empty
 * 
 * This method checks if the graintable is empty
 * 
 * @param gt graintable object to be checked
 * 
 * @return 0 or 1 indicates N or Y
 */
int graintable_is_empty(graintable *gt);

/**
 * @memberof graintable
 * @brief gets number of evelopes in graintable
 * 
 * This method gets the number of grains in graintable
 * 
 * @param gt graintable object to be checked
 * 
 * @return int the number of grains in graintable
 */
int graintable_get_len(graintable *gt);

/**
 * @memberof graintable
 * @brief update all timeout of grains
 * 
 * This method updates all timeout parameter of grains in the graintable
 * 
 * @param gt graintable object that stores grains 
 * @param n number of new samples stored in audio buffer
 */
void graintable_update_timeout(graintable *gt, int n);

/**
 * @memberof graintable
 * @brief print information of all grains in graintable
 * 
 * This method prints information of all grains in graintable for debugging
 * 
 * @param gt graintable object that stores grains 
 */
void graintable_print_all(graintable *gt);



