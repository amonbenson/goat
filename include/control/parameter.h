#pragma once

#include "control/modulator.h"
#include <stddef.h>



#define CONTROL_NUM_SLOTS 3

#define param(type, ...) control_parameter_get_ ## type(__VA_ARGS__)


/**
 * @struct control_parameter_slot
 * @brief a slot where a modulator can be attached
 */
typedef struct control_parameter_slot {
    control_modulator *mod; /**< the modulator */
    float amount; /**< the amount of influence (= factor) the modulator has */
} control_parameter_slot;

/**
 * @struct control_parameter
 * @brief a generic float parameter with a fixed number of slots to attach modulators to
 * 
 * For more detailed info on how the @ref control_parameter.value is calculated,
 * see @ref control_manager.control_manager_perform
 */
typedef struct control_parameter {
    char *name; /**< the name of the parameter */

    control_parameter_slot slots[CONTROL_NUM_SLOTS]; /**< the modulator slots */

    float offset; /**< the offset of the parameter. With no modulators attached, this becomes the value */
    float min; /**< the minimum value of the parameter */
    float max; /**< the maximum value of the parameter */
    float value; /**< the current computed value of the parameter */
    float default; /**< the default value for reset */

    struct control_parameter *next; /**< the next parameter in the list */
} control_parameter;

/**
 * @memberof control_parameter
 * @brief creates a new parameter
 * 
 * @param name the name of the parameter
 * @param default_value the initial value of the parameter
 * @param min the minimum value of the parameter
 * @param max the maximum value of the parameter
 * @return control_parameter* 
 */
control_parameter *control_parameter_new(const char *name, float default_value, float min, float max);

/**
 * @memberof control_parameter
 * @brief frees a parameter
 * 
 * @param p the parameter to free
 */
void control_parameter_free(control_parameter *p);


/**
 * @memberof control_parameter
 * @brief connects a modulator to a parameter
 * If the parameter already has a modulator attached, it will be replaced.
 * 
 * @param p the parameter
 * @param slot the slot to connect the modulator to
 * @param mod the modulator to connect
 */
void control_parameter_attach(control_parameter *p, size_t slot, control_modulator *mod);

/**
 * @memberof control_parameter
 * @brief disconnects a modulator from a parameter
 * If the parameter has no modulator attached, this function does nothing.
 * 
 * @param p the parameter
 * @param slot the slot to disconnect the modulator from
 */
void control_parameter_detach(control_parameter *p, size_t slot);

/**
 * @memberof control_parameter
 * @brief sets the amount of influence of a modulator on a parameter
 * 
 * @param p the parameter
 * @param slot the slot to set the amount of influence on
 * @param amount the amount of influence to set
 */
void control_parameter_amount(control_parameter *p, size_t slot, float amount);

/**
 * @memberof control_parameter
 * @brief sets the offset of a parameter
 * 
 * @param p the parameter
 * @param offset the offset to set
 */
void control_parameter_set(control_parameter *p, float offset);


/**
 * @memberof control_parameter
 * @brief gets the value of a parameter as a float
 * 
 * @param p the parameter
 * @return float the value of the parameter
 */
float control_parameter_get_float(control_parameter *p);

/**
 * @memberof control_parameter
 * @brief gets the value of a parameter cast as an integer
 * 
 * @param p the parameter
 * @return int the value of the parameter
 */
int control_parameter_get_int(control_parameter *p);
