/**
 * @file params.h
 * @author zeyu yang (zeyuuyang42@gmail.com)
 * @brief all constants in 
 * @version 0.2
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#define MAXTABLESIZE 4069 /**< maximum table size */
#define DELAYLINESIZE 262144  /**< delay line size, 262144 equal to 2^18, close to 6s under sample rate 44100 */
#define NUMACTIVEGRAIN 5 /**< maximum number of active grains */
#define ENVELOPEBUFSIZE 12  /**< size of the buffer for the envelopment of the grain */
#define PI M_PI /**< alternate pi definition */
