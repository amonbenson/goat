#include "granular/granular.h"

#include "util/mem.h"
#include "params.h"

granular *granular_new(void) {
    granular *g = malloc(sizeof(granular));
    if (!g) return NULL;

    // create the circular buffer
    g->buffer = circbuf_new(DELAYLINESIZE); 
    if (!g->buffer) return NULL;
    // add a single read tap. 
    if (!circbuf_readtap_add(g->buffer)) return NULL;
    g->buffer->readtaps->speed = 1.0f; 

    // create the graintable
    g->grains = graintable_new(MAXTABLESIZE); 
    if (!g->grains) return NULL;

    // create the evelopbuf
    g->evelopes = evelopbuf_new(ENVELOPEBUFSIZE); 
    if (!g->evelopes) return NULL;

    g->synth = synthesizer_new(NUMACTIVEGRAIN);
    if (!g->synth) return NULL;

    return g;
}


void granular_free(granular *g) {
    synthesizer_free(g->synth);
    evelopbuf_free(g->evelopes);
    graintable_free(g->grains);
    circbuf_free(g->buffer);
    free(g);
}


void granular_perform(granular *g, scheduler *s, float *in, float *out, int n) {
    grain* gn = NULL; // todo: remove this somehow
    evelope* ep = NULL; // todo: remove this somehow

    // Delayline load input stream
    circbuf_write_block(g->buffer, in, n); //load input stream into circbuf constantly @todo add parameter to stop and continue loading 
    
    // sample new grain and add into graintable
    if (s->fetchgrain == 0){
        printf("gransize %d\n", param(int, s->gransize));
        graintable_add_grain(g->grains, g->buffer, param(int, s->gransize), 0);
    }
    // post("graintable length: %d",graintable_get_len(g->grains));

    // update state of all grains
    graintable_update_timeout(g->grains, n);

    // fetch grain to synthesis output
    if (s->synthgrain == 0){
        // gn = graintable_check_grain(g->grains, gn, 0); // set delay to 0 is actully fetch the same grain except won't move it out
        do{
            gn = graintable_pop_grain(g->grains, gn);
        }while (gn->timeout <= 0);  

        ep = evelopbuf_check_evelope(g->evelopes, gn->evelope, gn->duration);
        synthesizer_active_grain(g->synth, gn, ep);
    }

    synthesizer_write_output(g->synth, out, n);
}
