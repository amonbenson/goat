#pragma once


#include "m_pd.h"


typedef struct _helloworld {
    t_object x_obj;
} t_helloworld;


void helloworld_bang(t_helloworld *x);

void *helloworld_new(void);

void helloworld_setup(void);
