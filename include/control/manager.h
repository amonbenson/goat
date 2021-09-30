/**
 * @file manager.h
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
#include "uthash/utlist.h"
#include "control/parameter.h"
#include "control/modulator.h"


/**
 * @struct control_manager
 * @brief manager to hold all parameters and modulators
 * 
 */
typedef struct {
    control_parameter *parameters; /**< list of all parameters */
    control_modulator *modulators; /**< list of all modulators */
} control_manager;


/**
 * @memberof control_manager
 * @brief create a new control manager
 * 
 * @return control_manager* a pointer to the new control manager or NULL if the allocation failed
 */
control_manager *control_manager_new();

/**
 * @memberof control_manager
 * @brief free a control manager
 * 
 * @param mgr the control manager to free
 */
void control_manager_free(control_manager *mgr);

/**
 * @memberof control_manager
 * @brief add a parameter
 * 
 * @param mgr the control manager to add the parameter to
 * @param name the name of the parameter
 * @param default_value the default value of the parameter
 * @param min the minimum value of the parameter
 * @param max the maximum value of the parameter
 * @return control_parameter* a pointer to the new parameter or NULL if the allocation failed
 */
control_parameter *control_manager_parameter_add(control_manager *mgr,
    const char *name,
    float default_value,
    float min,
    float max);

/**
 * @memberof control_manager
 * @brief remove a parameter
 * 
 * @param mgr the control manager to remove the parameter from
 * @param p the parameter to remove
 */
void control_manager_parameter_remove(control_manager *mgr, control_parameter *p);

/**
 * @memberof control_manager
 * @brief find a parameter by its name
 * 
 * @param mgr the control manager to search in
 * @param name the name of the parameter to find
 * @return control_parameter* a pointer to the parameter or NULL if not found
 */
control_parameter *control_manager_parameter_by_name(control_manager *mgr, const char *name);


/**
 * @memberof control_manager
 * @brief add a modulator
 * 
 * @param mgr the control manager to add the modulator to
 * @param name the name of the modulator
 * @param perform_method the method to call when the modulator performs
 * @param subclass_size the size of the subclass
 * @return control_modulator* a pointer to the new modulator or NULL if the allocation failed
 */
control_modulator *control_manager_modulator_add(control_manager *mgr,
    const char *name,
    control_modulator_perform_method perform_method,
    size_t subclass_size);

/**
 * @memberof control_manager
 * @brief remove a modulator
 * 
 * @param mgr the control manager to remove the modulator from
 * @param m the modulator to remove
 */
void control_manager_modulator_remove(control_manager *mgr, control_modulator *m);

/**
 * @memberof control_manager
 * @brief find a modulator by its name
 * 
 * @param mgr the control manager to search in
 * @param name the name of the modulator to find
 * @return control_modulator* a pointer to the modulator or NULL if not found
 */
control_modulator *control_manager_modulator_by_name(control_manager *mgr, const char *name);


/**
 * @memberof control_manager
 * @brief run the perform method on all modulators and parameters
 * 
 * @param mgr the control manager
 * @param in the input buffer
 * @param n the number of samples to process
 */
void control_manager_perform(control_manager *mgr, float *in, int n);
