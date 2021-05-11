#include "helloworld.h"


static t_class *helloworld_class;


void helloworld_bang(__attribute__((unused)) t_helloworld *x) {
    post("Hello world!!");
}

void *helloworld_new(void) {
    t_helloworld *x = (t_helloworld *) pd_new(helloworld_class);

    return (void *) x;
}

void helloworld_setup(void) {
    helloworld_class = class_new(gensym("helloworld"),
        (t_newmethod) helloworld_new,
        0,
        sizeof(t_helloworld),
        CLASS_DEFAULT,
        0);

    class_addbang(helloworld_class, helloworld_bang); 
}
