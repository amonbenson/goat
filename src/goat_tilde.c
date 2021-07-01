#include "goat_tilde.h"
#include "mem.h"


static t_class *goat_tilde_class;


void *goat_tilde_new(void) {
    goat_tilde *x = (goat_tilde *) pd_new(goat_tilde_class);
    if (!x) return NULL;

    x->out = outlet_new(&x->x_obj, &s_signal);
    if (!x->out) return NULL;

    return (void *) x;
}

void goat_tilde_free(goat_tilde *x) {
    outlet_free(x->out);
}

static t_int *goat_tilde_perform(t_int *w) {
    w++;

    __attribute__((unused)) goat_tilde *x = (goat_tilde *) *w++;
    t_sample *in = (t_sample *) *w++;
    t_sample *out = (t_sample *) *w++;
    int n = (int) *w++;
    int samplerate = (t_float) *w++;

    post("samples %d, rate %d: cloning signal %p -> %p", n, samplerate, in, out);

    // clone the signal
    if (in != out) memcpy(in, out, n * sizeof(t_sample));

    return w;
}

void goat_tilde_dsp(goat_tilde *x, t_signal **sp) {
    t_signal *in = sp[0];
    t_signal *out = sp[0];
    int n = in->s_n;
    int samplerate = in->s_sr;

    dsp_add(goat_tilde_perform, 5, x, in, out, n, samplerate);
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
