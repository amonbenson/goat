#pragma once

#include <stddef.h>


/**
 * @struct goat_config
 * @brief Configuration structure for the Goat engine.
 */
typedef struct goat_config {
    size_t sample_rate;
    size_t block_size;
} goat_config;
