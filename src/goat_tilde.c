#include "goat_tilde.h"
#include "goat.h"
#include "control/manager.h"
#include "util/mem.h"


static t_class *goat_tilde_class;


void *goat_tilde_new(void) {
    goat_tilde *x = (goat_tilde *) pd_new(goat_tilde_class);
    if (!x) return NULL;

    x->out = outlet_new(&x->x_obj, &s_signal);
    if (!x->out) return NULL;

    goat_config config = {
        .sample_rate = (size_t) sys_getsr(),
        .block_size = sys_getblksize()
    };
    x->g = goat_new(&config);

    return (void *) x;
}

void goat_tilde_free(goat_tilde *x) {
    outlet_free(x->out);
    goat_free(x->g);
}

static control_parameter *goat_tilde_validate_parameter(goat_tilde *x, const char *name) {
    control_parameter *param = control_manager_parameter_by_name(x->g->cfg.mgr, name);
    if (param == NULL) error("goat~: unknown parameter %s", name);
    return param;
}

static control_modulator *goat_tilde_validate_modulator(goat_tilde *x, const char *name) {
    control_modulator *mod = control_manager_modulator_by_name(x->g->cfg.mgr, name);
    if (mod == NULL) error("goat~: unknown modulator %s", name);
    return mod;
}

static int goat_tilde_validate_slot(int slot) {
    if (slot < 0 || slot >= CONTROL_NUM_SLOTS) {
        error("goat~: slot %d out of range", slot);
        return -1;
    }
    return slot;
}

void goat_tilde_param_set(goat_tilde *x, t_symbol *paramname, t_float value) {
    control_parameter *param;

    if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;

    control_parameter_set(param, value);
}

void goat_tilde_param_amount(goat_tilde *x, t_symbol *paramname, t_float fslot, t_float value) {
    control_parameter *param;
    int slot;

    if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;
    if ((slot = goat_tilde_validate_slot(fslot)) == -1) return;

    control_parameter_amount(param, slot, value);
}

void goat_tilde_param_attach(goat_tilde *x, __attribute__((unused)) t_symbol *s, int argc, t_atom *argv) {
    control_parameter *param;
    control_modulator *mod;
    int slot;

    t_symbol *paramname = atom_getsymbolarg(0, argc, argv);
    float fslot = atom_getfloatarg(1, argc, argv);
    t_symbol *modname = atom_getsymbolarg(2, argc, argv);

    if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;
    if ((mod = goat_tilde_validate_modulator(x, modname->s_name)) == NULL) return;
    if ((slot = goat_tilde_validate_slot(fslot)) == -1) return;

    if (param->slots[slot].mod == mod) {
        error("goat~: already attached");
        return;
    }

    control_parameter_attach(param, slot, mod);
}

void goat_tilde_param_detach(goat_tilde *x, t_symbol *paramname, t_floatarg fslot) {
    control_parameter *param;
    int slot;

    if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;
    if ((slot = goat_tilde_validate_slot(fslot)) == -1) return;

    if (param->slots[slot].mod == NULL) {
        error("goat~: already detached");
        return;
    }

    control_parameter_detach(param, slot);
}

void goat_tilde_param_post(goat_tilde *x) {
    control_parameter *param;
    int i;

    post("PARAMETERS:");
    LL_FOREACH(x->g->cfg.mgr->parameters, param) {
        startpost("    %s: %.2f <- %.2f",
            param->name,
            control_parameter_get_float(param),
            param->offset);
        for (i = 0; i < CONTROL_NUM_SLOTS; i++) {
            if (param->slots[i].mod) {
                startpost(" + %s * %.2f",
                    param->slots[i].mod->name,
                    param->slots[i].amount);
            }
        }
        endpost();
    }
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
        (t_method) goat_tilde_param_set,
        gensym("param-set"),
        A_SYMBOL,
        A_FLOAT,
        A_NULL);
    class_addmethod(goat_tilde_class,
        (t_method) goat_tilde_param_amount,
        gensym("param-amount"),
        A_SYMBOL,
        A_FLOAT,
        A_FLOAT,
        A_NULL);
    // there seems to be a bug in pure data where a float followed by a symbol argument
    // causes Pd to crash on windows. Therefore, we use a gimme instead.
    class_addmethod(goat_tilde_class,
        (t_method) goat_tilde_param_attach,
        gensym("param-attach"),
        A_GIMME,
        A_NULL);
    class_addmethod(goat_tilde_class,
        (t_method) goat_tilde_param_detach,
        gensym("param-detach"),
        A_SYMBOL,
        A_FLOAT,
        A_NULL);
    class_addmethod(goat_tilde_class,
        (t_method) goat_tilde_param_post,
        gensym("param-post"),
        A_NULL);

    class_addmethod(goat_tilde_class,
        (t_method) goat_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);
    CLASS_MAINSIGNALIN(goat_tilde_class, goat_tilde, f);
}
