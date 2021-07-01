/**
 * @file granular.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief Granular Synthesizer
 * @version 0.1
 * @date 2021-07-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "circbuf.h"


typedef struct {
    circbuf *buffer;
} granular;

granular *granular_new();

void granular_free(granular *g);

void granular_perform(granular *g, float *in, float *out, int n);
