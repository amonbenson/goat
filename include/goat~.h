#pragma once


#include "m_pd.h"


typedef struct _goat_tilde {
    t_object x_obj;
    t_float *f;

    t_outlet *out;
} t_goat_tilde;


void *goat_tilde_new(void);

void goat_tilde_free(t_goat_tilde *x);

void goat_tilde_dsp(t_goat_tilde *x, t_signal **sp);

void goat_tilde_setup(void);
