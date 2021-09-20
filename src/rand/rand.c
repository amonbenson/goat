#include "rand/rand.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "m_pd.h"
//








void rand_setDist(t_rand *x, float mu, float sigma){
    x->mu=mu;
    x->sigma=sigma;
}

void rand_setSeed(t_rand *x){
    srand(x->rand_intSeed);
    x->rand_intSeed=rand();
}

void rand_onBangMsg(t_rand *x){
    rand_setSeed(x);
    x->rand_num = rand_nn_perform(x);
    post("[rand] banging: %f",x->rand_num);
    outlet_float(x->outRand,x->rand_num);

}

void rand_onMu(t_rand *x, t_floatarg mu){
    rand_setDist(x,mu,x->sigma);
}

void rand_onSigma(t_rand *x, t_floatarg sigma){
    rand_setDist(x,x->mu,sigma);
}

void *rand_new(t_floatarg mu, t_floatarg sigma){
    t_rand *x = (t_rand *) pd_new(rand_class);

    x->rand_intSeed=time(NULL); 

    rand_setDist(x,mu,sigma);

    x->outRand = outlet_new(&x->x_obj, &s_float);
    x->in_mu = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("ExpVal_Mu"));
    x->in_sigma = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("StdDev_Sigma"));
    return (void *)x; //double casting?! from generic to t_rand to generic?!
}

void rand_free(t_rand *x){
    outlet_free(x->outRand);
    inlet_free(x->in_mu);
    inlet_free(x->in_sigma);
}

void rand_setup(void){
    rand_class = class_new(
        gensym("rand"),
        (t_newmethod)rand_new,
        (t_method) rand_free,
        sizeof(t_rand),
        CLASS_DEFAULT,
        A_DEFFLOAT, //mu
        A_DEFFLOAT, //sigma
        0);
        class_addbang(rand_class, (t_method)rand_onBangMsg);
        class_addmethod(rand_class,
            (t_method)rand_onMu,
            gensym("ExpVal_Mu"),
            A_DEFFLOAT,
            0);
        class_addmethod(rand_class,
            (t_method)rand_onSigma,
            gensym("StdDev_Sigma"),
            A_DEFFLOAT,
            0);        
}

float rand_nn_perform(t_rand *x) { //mu: Expectation value; sigma: standard deviation
    //float *list = malloc(sizeof(float)*count);
    //if (!list) return NULL; //needs to be freed 
    

    float s, t, a, b; //Paramteres defining the distribution
    s = 0.449871;
    t = -0.386595;
    a = 0.196;
    b = 0.25472;
    
    float u, v, w, y, Q; //helper variables for the algorithm
    int num_rej=0; //limits the loop.

    //srand(time(0)); //new seed for rand()
    
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


/* testfunction 
int main(void){
   float a;
   a = rand_nn_perform(65,25);
   printf("%f",a);
   return 0; 
}
*/