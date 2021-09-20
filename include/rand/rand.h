#include <stdlib.h>
#include "m_pd.h"

static t_class *rand_class;

typedef struct _rand {
    t_object    x_obj;
    t_float     mu, sigma;
    t_float     rand_num;
    t_int       rand_intSeed;
    t_inlet     *in_mu, *in_sigma;
    t_outlet    *outRand;
}t_rand; //struct _rand will be referred to as t_rand

void rand_setDist(t_rand *x, float mu, float sigma);

void rand_setSeed(t_rand *x);

void rand_onBangMsg(t_rand *x);

void rand_onMu(t_rand *x, t_floatarg mu);

void rand_onSigma(t_rand *x, t_floatarg sigma);

void *rand_new(t_floatarg mu, t_floatarg sigma);

void rand_free(t_rand *x);

void rand_setup(void);

float rand_nn_perform(t_rand *x);