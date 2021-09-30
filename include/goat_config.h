/**
 * @file goat_config.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <stddef.h>
#include "control/manager.h"


/**
 * @struct goat_config
 * @brief Configuration structure for the Goat engine.
 */
typedef struct {
    size_t sample_rate; /**< audio samples per second */
    size_t block_size; /**< size of one audio block. The vector size n of the perform method might be smaller!! */
    control_manager *mgr; /**< global control manager */
} goat_config;
