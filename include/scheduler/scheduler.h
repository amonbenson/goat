/**
 * @file scheduler.h
 * @author zeyu yang (zeyuyang42@163.com)
 * @brief scheduler is a instance that store all user adjustable configs, like gran_size, inter_onset. 
 *        granular polls the statue in scheduler at certain polling rate to get configs for grain arragment.
 *        At version 0.1 all configs are temporarily constant, but little randomness will be involved for justifying the correctness of current granular
 *        functions for adjusting all configs with a human friendly method will come at version 0.2
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stdio.h>
#include <stdlib.h> // for the rand function
#include <time.h>   //for seed

#include "util/mem.h"
#include "util/util.h"
// #include "util/circbuf.h"

#include "m_pd.h" // add for post function, remove this after debuging

#include "params.h"


/**
 * @struct scheduler
 * @brief circular buffer class
 * 
 * The circular buffer class contains a data array, the buffer size and references to the corresponding
 * read and write taps.
 */
typedef struct {
    // configs from pd
    int streamsize; // 64 samples for now 
    int samplerate; // 44100 samples for now 

    // basic user adjustable configs
    int gransize;       /**< the size of sampled grains also for corresponding evelope*/
    int interonset;     /**< the onset difference between two grains that been sampled */
    int maxinteronset;  /**< the maximun onset difference between two grains to be synthesized */
    int mininteronset;  /**< the minimun onset difference between two grains to be synthesized */
        // evelop related
    int eveloptype;     /**< type of evelop used for grain generation procedure */


    // configs that changed at each dsp routine
    int fetchgrain;
    int synthgrain;

    // advance user adjustable configs 
    int getpitch;       /**< enable the pitch detection or not, 0 for disable */
    int getenergy;      /**< enable the energy detection or not, 0 for disable */
    float pitch;        /**< wished pitch of transformed grain, e.g. 440Hz for all upcoming grains*/
    float energy;       /**< wished energy of transformed grain, e.g. 0.5 for all upcoming grains */
    float pitchratio;   /**< wished energy of transformed grain, e.g. 2*higher(octave) for all upcoming grains*/
    float energyratio;  /**< wished energy of transformed grain, e.g. 2*louder for all upcoming grains */

}scheduler, *p_scheduler;


/**
 * @struct scheduler
 * @brief new a scheduler instance
 * 
 * Chose a random number between maxinteronset and mininteronset as the interonset of next grain
 */
scheduler *scheduler_new(void);


/**
 * @struct scheduler
 * @brief get the next interonset 
 * 
 * Chose a random number between maxinteronset and mininteronset as the interonset of next grain
 */
void scheduler_free(scheduler *sd);


/**
 * @struct scheduler
 * @brief get the next interonset 
 * 
 * Chose a random number between maxinteronset and mininteronset as the interonset of next grain
 */
int scheduler_get_next_interonset(int max, int min, int slot); 


/**
 * @struct scheduler
 * @brief update configs at each dsp routin
 * 
 * 
 */
void scheduler_perform(scheduler *sd);



/**
 * @struct scheduler
 * @brief update configs for controlling ot fetch and synth process
 * 
 * 
 */
void scheduler_update_counter(scheduler *sd);