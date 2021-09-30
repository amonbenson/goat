/**
 * @file evelopbuf.h
 * @author zeyu yang (zeyuuyang42@gmail.com)
 * @brief evelope and evelopbuf class, save & search & generate evelopes
 * @version 0.2
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stdio.h>
#include <stddef.h>

#include "params.h"
#include "util/mem.h"
#include "util/util.h"
#include "util/circbuf.h"

#include "m_pd.h" // add for post function, remove this after debuging

/**
 * @struct evelope
 * @brief evelope class 
 * 
 * Store data of a evelope and its parameters. 
 * 4 types of evelope supported: parabolic, trapezoidal, raised_cosine_bell, no_evelope
 * Each of them with arbitrary length, amplitude, attacksamples or releasesamples 
 * 
 */
typedef struct {
    float *data;         /**< The stored evelope data itself */
    int type;            /**< The type of evelope */
    int length;          /**< The length of evelope */
    int attacksamples;   /**< The number of attack samples */
    int releasesamples;  /**< The number of release samples */
} evelope, *p_evelope; /**< pointer to an envelope */


/**
 * @struct evelopbuf
 * @brief evelope buffer class
 * 
 * The evelope buffer class contains a data array, 
 * the buffer size and two indicators of front and rear of the queue.
 */
typedef struct {
    p_evelope *data;  /**< The stored evelope structure itself */
    int size;         /**< The buffer size */
    int front;        /**< The indicator to the front position */
    int rear;         /**< The indicator to the rear position */

} evelopbuf;


/**
 * @memberof evelope
 * @brief generate evelope  
 * 
 * This method creates a new evelope object
 * 
 * @param ep the evelopbuf object to save generated evelope
 * @param type the type of to the be generated evelope
 * @param length the length of the to be generated evelope
 * @param amplitude the maximum amplitude of the to be generated evelope
 * @param attacksamples the attack samples of the to be generated evelope
 * @param releasesamples the release samples of the to be generated evelope
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelope_new(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples);

/**
 * @memberof evelope
 * @brief free a evelope object
 * 
 * This method frees an evelope object
 * 
 * @param ep evelope object to be freed
 */
void evelope_free(evelope* ep);

/**
 * @memberof evelope
 * @brief create parabolic evelope
 * 
 * This method creates evelope using parabolic algorithm with adjustable parameters
 * 
 * @param ep the evelopbuf object to save generated evelope
 * @param type the type of to the be generated evelope
 * @param length the length of the to be generated evelope
 * @param amplitude the maximum amplitude of the to be generated evelope
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelope_gen_parabolic(evelope* ep, int type, int length, float amplitude);

/**
 * @memberof evelope
 * @brief create trapezoidal evelope 
 * 
 * This method creates evelope using trapezoidal algorithm with adjustable parameters
 * 
 * @param ep the evelopbuf object to save generated evelope
 * @param type the type of to the be generated evelope
 * @param length the length of the to be generated evelope
 * @param amplitude the maximum amplitude of the to be generated evelope
 * @param attacksamples the attack samples of the to be generated evelope
 * @param releasesamples the release samples of the to be generated evelope
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelope_gen_trapezoidal(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples);

/**
 * @memberof evelope
 * @brief create raised_cosine_bell evelope 
 * 
 * This method creates evelope using raised cosine bell algorithm with adjustable parameters
 * 
 * @param ep the evelopbuf object to save generated evelope
 * @param type the type of to the be generated evelope
 * @param length the length of the to be generated evelope
 * @param amplitude the maximum amplitude of the to be generated evelope
 * @param attacksamples the attack samples of the to be generated evelope
 * @param releasesamples the release samples of the to be generated evelope
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelope_gen_raised_cosine_bell(evelope* ep, int type, int length, float amplitude, int attacksamples, int releasesamples);

/**
 * @memberof evelope
 * @brief create plane evelope
 * 
 * This method creates plane evelope.
 * No evelope actually except amplitude adjustion.
 * 
 * @param ep the evelopbuf object to save generated evelope
 * @param type the type of the to be generated evelope
 * @param length the length of the to be generated evelope
 * @param amplitude the maximum amplitude of the to be generated evelope
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelope_gen_no_evelope(evelope* ep, int type, int length, float amplitude);


/**
 * @memberof evelopbuf
 * @brief This method creates a new evelopbuf object
 * 
 * @param size size of evelope buffer
 * 
 * @return evelopbuf* a reference to the evelopbuf object or `NULL` if failed
 */
evelopbuf *evelopbuf_new(int size);

/**
 * @memberof evelopbuf
 * @brief This method frees an evelopbuf object
 * 
 * @param eb evelopbuf object to be freed
 */
void evelopbuf_free(evelopbuf *eb);

/**
 * @memberof evelopbuf
 * @brief add evelope object into evelopbuf
 * 
 * This method adds an evelope object to the rear of evelope buffer
 * If the buffer is full the front one will be removed
 * 
 * @param eb evelopbuf object to store evelope object
 * @param type the type of the to be added evelope
 * @param length the length of the to be added evelope
 * @param attacksamples attack time in samples
 * @param releasesamples release time in samples
 */ 
void evelopbuf_add_evelope(evelopbuf *eb, int type, int length, int attacksamples, int releasesamples); // todo: add all parameters 

/**
 * @memberof evelopbuf
 * @brief pops evelope object out of buffer
 * 
 * This method pop evelope object out of buffer when the buffer is not empty
 * 
 * @param eb evelopbuf object where evelope object will be popped
 * @param ep evelope object to store popped evelope object
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelopbuf_pop_evelope(evelopbuf *eb, evelope* ep);

/**
 * @memberof evelopbuf
 * @brief checks all evelopes in evelopbuf
 * 
 * This method checks if requested evelope in evelope buffer
 * Fetch and return it if found, else create it and return it
 * 
 * @param eb evelopbuf object to be checked
 * @param type the type of the requested evelope
 * @param length the length of the requested evelope
 * @param attacksamples number for samples for envelope attack
 * @param releasesamples number of samples for envelope release
 * 
 * @return evelope* a reference to the evelope object or `NULL` if failed
 */
evelope *evelopbuf_check_evelope(evelopbuf *eb, int type, int length, int attacksamples, int releasesamples);

/**
 * @memberof evelopbuf
 * @brief checks if the evelopbuf is full
 * 
 * This method checks if the evelopbuf is full
 * 
 * @param eb evelopbuf object to be checked
 * 
 * @return 0 or 1 indicates N or Y
 */
int evelopbuf_is_full(evelopbuf *eb);

/**
 * @memberof evelopbuf
 * @brief checks if the evelopbuf is empty
 * 
 * This method checks if the evelopbuf is empty
 * 
 * @param eb evelopbuf object to be checked
 * 
 * @return 0 or 1 indicates N or Y
 */
int evelopbuf_is_empty(evelopbuf *eb);

/**
 * @memberof evelopbuf
 * @brief gets number of evelopes in evelopbuf
 * 
 * This method gets the number of evelopes in evelope buffer
 * 
 * @param eb evelopbuf object to be checked
 * 
 * @return int the number of evelopes in evelope buffer
 */
int evelopbuf_get_len(evelopbuf *eb);



