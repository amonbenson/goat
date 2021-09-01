#include "goat_tilde.h"
#include "goat.h"
#include "util/mem.h"


static t_class *goat_tilde_class;


void *goat_tilde_new(void) {
    goat_tilde *x = (goat_tilde *) pd_new(goat_tilde_class);
    if (!x) return NULL;

    x->out = outlet_new(&x->x_obj, &s_signal);
    if (!x->out) return NULL;

    x->g = goat_new();

    return (void *) x;
}

void goat_tilde_free(goat_tilde *x) {
    outlet_free(x->out);
    goat_free(x->g);
}

static t_int *goat_tilde_perform(t_int *w) {
    goat_tilde *x = (goat_tilde *) w[1];
    t_sample *in = (t_sample *) w[2];
    t_sample *out = (t_sample *) w[3];
    int n = (int) w[4];

    // invoke the main algorithm
    goat_perform(x->g, in, out, n);

    // debug info
    // post("write %p -> %d, read %f -> %p\n",
    //     in,
    //     x->g->gran->buffer->writetap.position,
    //     x->g->gran->buffer->readtaps->position,
    //     out);

    return &w[5];
}

void goat_tilde_dsp(goat_tilde *x, t_signal **sp) {
    t_sample *in = sp[0]->s_vec;
    t_sample *out = sp[1]->s_vec;
    int n = sp[0]->s_n;

    dsp_add(goat_tilde_perform, 4, x, in, out, n);
}

void goat_tilde_setup(void) {
    goat_tilde_class = class_new(gensym("goat~"),
        (t_newmethod) goat_tilde_new,
        (t_method) goat_tilde_free,
        sizeof(goat_tilde),
        CLASS_DEFAULT,
        0);

    class_addmethod(goat_tilde_class,
        (t_method) goat_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);

    CLASS_MAINSIGNALIN(goat_tilde_class, goat_tilde, f);
}
