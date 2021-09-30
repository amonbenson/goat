/**
 * @file goat_tilde.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief G.O.A.T Pure Data External
 * @version 0.1
 * @date 2021-07-01
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "m_pd.h"

#include "goat.h"

/**
 * @struct goat_tilde
 * @brief main pure data external
 * 
 * This is the main pure data interface
 */
typedef struct {
    t_object x_obj; /**< parent Pure Data object */
    t_float *f; /**< fallback field for the main signal inlet */

    t_outlet *sigout; /**< main signal outlet */
    t_outlet *dataout; /**< main data outlet */

    goat *g; /**< pointer to the goat object */

} goat_tilde;


/**
 * @memberof goat_tilde
 * @brief creates a new goat_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *goat_tilde_new(void);

/**
 * @memberof goat_tilde
 * @brief frees an existing goat_tilde object and all of its subclasses
 * 
 * @param x the goat object to be freed. Must not be `NULL`
 */
void goat_tilde_free(goat_tilde *x);

/**
 * @memberof goat_tilde
 * @brief get a single or all parameter values from the goat object
 * 
 * @param x the goat object
 * @param paramname the name of the parameter to get. If `NULL` or empty, all parameters are returned
 */
void goat_tilde_param_get(goat_tilde *x, t_symbol *paramname);

/**
 * @memberof goat_tilde
 * @brief updates a parameter's value
 * 
 * @param x the goat object
 * @param paramname the name of the parameter to be updated
 * @param value the offset value to be updated
 */
void goat_tilde_param_set(goat_tilde *x, t_symbol *paramname, t_float value);

/**
 * @memberof goat_tilde
 * @brief updates a parameter slot's amount of influence
 * 
 * @param x the goat object
 * @param paramname the name of the parameter to be updated
 * @param fslot the slot to be updated
 * @param value the amount to be updated
 */
void goat_tilde_param_amount(goat_tilde *x, t_symbol *paramname, t_float fslot, t_float value);

/**
 * @memberof goat_tilde
 * @brief connects a modulator to a parameter.
 * Any other modulator on that slot will be disconnected.
 * Due to a bug in the windows version of Pure Data, this method had to be implemented using
 * A_GIMME instead of the parameter list A_SYMBOL, A_FLOAT, A_SYMBOL.
 * 
 * @param x the goat object
 * @param s unused symbol representation of the following arguments
 * @param argc the number of arguments
 * @param argv the arguments in the order: param name, slot, modulator name
 */
void goat_tilde_param_attach(goat_tilde *x, t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof goat_tilde
 * @brief disconnects a modulator from a parameter
 * 
 * @param x the goat object
 * @param paramname the name of the parameter to be updated
 * @param fslot the slot to be updated
 */
void goat_tilde_param_detach(goat_tilde *x, t_symbol *paramname, t_float fslot);

/**
 * @memberof goat_tilde
 * @brief posts all parameters to the debug console
 * 
 * @param x the goat object
 */
void goat_tilde_param_post(goat_tilde *x);

/**
 * @memberof goat_tilde
 * @brief resets all the parameters to default and detaches modulators
 * 
 * @param x the goat object
 */
void goat_tilde_param_reset(goat_tilde *x);

/**
 * @memberof goat_tilde
 * @brief sets up the dsp tree for a given goat_tilde object
 * 
 * @param x the goat object
 * @param sp the signal pointer provided by pure data
 */
void goat_tilde_dsp(goat_tilde *x, t_signal **sp);

/**
 * @related goat_tilde
 * @brief sets up the goat~ external
 */
void goat_tilde_setup(void);
