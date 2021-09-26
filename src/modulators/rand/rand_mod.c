#include "modulators/rand/rand_mod.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "util/util.h"
#include "m_pd.h"

//
rand_mod *rand_mod_new(goat_config *cfg, const char *name){
    rand_mod *rm = (rand_mod *) control_manager_modulator_add(cfg->mgr,
        name,
        (control_modulator_perform_method) rand_mod_perform,
        sizeof(rand_mod));
    
    rm->cfg = cfg;
    rm->seed=time(NULL);
    rm->time=0.0f;

    char namebuf[32];

    snprintf(namebuf, sizeof(namebuf), "%s.frequency", name);
    rm->freq = control_manager_parameter_add(cfg->mgr,
        namebuf, 1.0f, 0.01f, 689.0f ); // 689=44100/64=Samplingrate/Blocksize

    snprintf(namebuf, sizeof(namebuf), "%s.value", name);
    rm->mu = control_manager_parameter_add(cfg->mgr,
        namebuf, 100.0f, 0.01f, 20000.0f); //TODO: allow negatives... because modulators are summative

    snprintf(namebuf, sizeof(namebuf), "%s.variation", name);
    rm->sigma = control_manager_parameter_add(cfg->mgr,
        namebuf, 5.0f, 0.0001f, 10000.0f);

    return rm;
}


void rand_mod_free(rand_mod *rm){
    control_manager_parameter_remove(rm->cfg->mgr, rm->freq);
    control_manager_parameter_remove(rm->cfg->mgr, rm->mu);
    control_manager_parameter_remove(rm->cfg->mgr, rm->sigma);

    control_manager_modulator_remove(rm->cfg->mgr, &rm->super);
};

void rand_mod_perform(rand_mod *rm, __attribute__((unused)) float *in, int n){
    float a = fmod(rm->time, 1/control_parameter_get_float(rm->freq)); //!< Modulus of elapsed time and period of random numbers
    float b = (float) n / (float) rm->cfg->sample_rate; //!< Blocksize/Samplerate=time intervall between blocks
    if ( a < b ){
        rand_setSeed(rm);
        rm->rand_num = rand_nn(rm); //!< perform algorithm
        rm->super.value = rm->rand_num;
        //post("[rand] success: %f",rm->rand_num);//debugging post
        rm->time = 0.0; //!< reset timer
        
    }

    rm->time += b; //!< time increment per processed block
   // post("[rand] wait, time: %f",rm->time);
}


void rand_setSeed(rand_mod *rm){
    srand(rm->seed);
    rm->seed=rand();
}


float rand_nn(rand_mod *rm) { //mu: Expectation value; sigma: standard deviation

    float s, t, a, b; //Paramteres defining the distribution
    s = 0.449871;
    t = -0.386595;
    a = 0.196;
    b = 0.25472;
    
    float u, v, w, y, Q; //helper variables for the algorithm
    int num_rej=0; //limits the loop.
    
    while (num_rej <= 10) {
        u = (float)rand()/RAND_MAX;
        v = 1.7156*((float)rand()/RAND_MAX-0.5);
        w = u - s;
        y = fabs(v) - t;
        Q = w*w + y*(a*y-b*w);
        if (Q < 0.27597) {
            return (v/u*0.5*control_parameter_get_float(rm->sigma)+control_parameter_get_float(rm->mu));
        }
        if (Q>0.27846) {
            num_rej++;
            continue;
        }
        if (v*v > -4*u*u*log(u)) {
            num_rej++;
            continue;
        }
        else return (v/u*0.5*control_parameter_get_float(rm->sigma)+control_parameter_get_float(rm->mu));
    }
return 666; //should never be reached :O
}


