#include "control/modulators/rand/rand_mod.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

//
rand_mod *rand_mod_new(control_manager *mgr, const char *name){
    rand_mod *rm = (rand_mod *) control_manager_modulator_add(mgr,
        name,
        (control_modulator_perform_method) rand_mod_perform,
        sizeof(rand_mod));

    rm->mu= 100.0f; // TODO: currently unused
    rm->sigma=5.0f;
    rm->seed=time(NULL)
    return rm;
}


void rand_mod_free(rand_mod *rm, control_manager *mgr){
    control_manager_modulator_remove(mgr, &rm->super);
};

void rand_mod_perform(rand_mod *rm, __attribute__((unused)) float *in, __attribute__((unused)) int n){
    rand_setSeed(rm);
    rm->rand_num = rand_nn(rm);
    rm->super.value = rm->rand_num;
}


void rand_setSeed(rand_mod *rm){
    srand(rm->rand_intSeed);
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
            return (v/u*0.5*x->sigma+x->mu);
        }
        if (Q>0.27846) {
            num_rej++;
            continue;
        }
        if (v*v > -4*u*u*log(u)) {
            num_rej++;
            continue;
        }
        else return (v/u*0.5*x->sigma+x->mu);
    }
return 666;
}


