#pragma once

#include <stddef.h>


struct control_modulator;
/**
 * @memberof control_modulator
 * @brief perform callback function for a modulator
 * The method needs to set the control_modulator.value variable
 * 
 * @param mod the modulator
 * @param in the input buffer
 * @param n the number of samples
 */
typedef void (*control_modulator_perform_method)(struct control_modulator *mod, float *in, int n);

/**
 * @struct control_modulator
 * @brief a generic modulator interface
 * 
 * This struct can be used as a base class for any specific modulator
 */
typedef struct control_modulator {
    char *name; /**< the name of the modulator */
    control_modulator_perform_method perform_method; /**< the perform callback function */

    float value; /**< the current value of the modulator */

    struct control_modulator *next; /**< the next modulator in the list */
} control_modulator;


/**
 * @memberof control_modulator
 * @brief create a new modulator
 * More specifically, not only the modulator itself, but also memory for the subclass struct is
 * allocated. Therefore, subclasses can store additional data.
 * 
 * @param name the name of the modulator
 * @param perform_method the perform callback function
 * @param subclass_size the size of the subclass to allocate memory for
 * @return control_modulator* a pointer to the new modulator or NULL if memory allocation failed
 */
control_modulator *control_modulator_new(const char *name,
        control_modulator_perform_method perform_method,
        size_t subclass_size);

/**
 * @memberof control_modulator
 * @brief free a modulator
 * 
 * @param modulator the modulator to free
 */
void control_modulator_free(control_modulator *modulator);
