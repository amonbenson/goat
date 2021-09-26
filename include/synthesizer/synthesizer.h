/**
 * @file synthesizer.h
 * @author zeyu yang (zeyuuyang42@gmail.com)
 * @brief the final overlap & add processing that writes all active grains into out stream
 * @version 0.2
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

/**
 * @struct activategrain
 * @brief activategrain class contains data of activate grain
 * 
 * The activategrain class contains samples and basic parameters of an activate grain 
 * And other behaviour controll parameters
 */
typedef struct {
    float *data;   /**< The stored activate grain data itself */
    int pos;       /**< The position to read this activate grain */
    int length;    /**< The position to read this activate grain */
    int repeat;    /**< Wether use this grain repeatly */
} activategrain, *p_activategrain; /**< a pointer to an active grain */


/**
 * @struct synthesizer
 * @brief synthesizer class contains all activate grain objects
 * 
 * The synthesizer class contains all active grain and write them to the dsp out stream at each routine
 */
typedef struct {
    p_activategrain *data; /**< The stored data itself */
    int length;            /**< The size of synthesizer */
} synthesizer;


/**
 * @memberof activategrain
 * @brief creates a activategrain object
 * 
 * This method creates an activategrain object
 * 
 * @param gn the grain object contains information for activation
 * @param ep the grain object contains evelope
 * @param repeat whether remove this activate grain after reading throught it 
 * 
 * @return activategrain* a reference to the activategrain object or `NULL` if failed
 */
activategrain *activategrain_new(grain* gn, evelope* ep, int repeat);

/**
 * @memberof activategrain
 * @brief frees an activategrain object
 * 
 * This method frees an activategrain object
 * 
 * @param ag the activategrain object to be freed
 */
void activategrain_free(activategrain *ag);


/**
 * @memberof synthesizer
 * @brief creates a synthesizer object
 * 
 * This method creates a synthesizer object
 * 
 * @param length the number of maximun simulteneuly activate grains
 * 
 * @return synthesizer* a reference to the synthesizer object or `NULL` if failed
 */
synthesizer *synthesizer_new(int length);

/**
 * @memberof synthesizer
 * @brief frees a synthesizer object
 * 
 * This method frees a synthesizer object
 * 
 * @param syn the synthesizer object to be freed
 */
void synthesizer_free(synthesizer *syn);

/**
 * @memberof synthesizer
 * @brief active a grain
 * 
 * This method active a grain by reading it from buffer and mupltiply with the evelope
 * 
 * @param syn the synthesizer object that stores activate grains
 * @param gn the grain object contains information for activation
 * @param ep the grain object contains evelope
 */
void synthesizer_active_grain(synthesizer *syn, grain* gn, evelope* ep);

/**
 * @memberof synthesizer
 * @brief write out stream
 * 
 * This method writes activate grains to out stream at each dsp routine
 * 
 * @param syn the synthesizer object that stores activate grains
 * @param out the output buffer to be writen
 * @param n number of samples to write
 */
void synthesizer_write_output(synthesizer *syn, float *out, int n);
