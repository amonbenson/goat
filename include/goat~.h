/**
 * @file goat~.h
 * @author Amon Benson
 * @brief G.O.A.T Pure Data External
 * @version 0.1
 * @date 2021-07-01
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once


#include "m_pd.h"


/**
 * @brief main pure data external
 * 
 * This is the main pure data interface
 */
typedef struct _goat_tilde {
    t_object x_obj; /**< parent Pure Data object */
    t_float *f; /**< fallback field for the main signal inlet */

    t_outlet *out; /**< main signal outlet */
} t_goat_tilde;


/**
 * @related t_goat_tilde
 * @brief creates a new t_goat_tilde object
 * 
 * @return void* a pointer to the new object or NULL if the creation failed
 */
void *goat_tilde_new(void);

/**
 * @related t_goat_tilde
 * @brief frees an existing t_goat_tilde object and all of its subclasses
 * 
 * @param x the goat object to be freed. Must not be NULL
 */
void goat_tilde_free(t_goat_tilde *x);

/**
 * @related t_goat_tilde
 * @brief sets up the dsp tree for a given t_goat_tilde object
 * 
 * @param x the goat object
 * @param sp the signal pointer provided by pure data
 */
void goat_tilde_dsp(t_goat_tilde *x, t_signal **sp);

/**
 * @related t_goat_tilde
 * @brief sets up the goat~ external
 */
void goat_tilde_setup(void);
