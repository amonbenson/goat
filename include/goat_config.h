#pragma once

#include <stddef.h>
#include "control/manager.h"


/**
 * @struct goat_config
 * @brief Configuration structure for the Goat engine.
 */
typedef struct goat_config {
    size_t sample_rate; /**< audio samples per second */
    size_t block_size; /**< size of one audio block. The vector size n of the perform method might be smaller!! */
    control_manager *mgr; /**< global control manager */
} goat_config;
