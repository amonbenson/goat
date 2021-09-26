#include "granular/granular.h"

#include "util/mem.h"
#include "params.h"

granular *granular_new(void) {
    granular *g = malloc(sizeof(granular));
    if (!g) return NULL;

    g->buffer = circbuf_new(DELAYLINESIZE, NUMACTIVEGRAIN);
    if (!g->buffer) return NULL;

    g->grains = graintable_new(MAXTABLESIZE); 
    if (!g->grains) return NULL;

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
    grain* gn;
    evelope* ep;

    // Delayline load input stream
    circbuf_write_block(g->buffer, in, n); //load input stream into circbuf constantly @todo add parameter to stop and continue loading 
    
    // sample new grain and add into graintable
    if (s->dofetch){
        float speed = semitonefact(param(float, s->grainpitch));
        float duration = param(float, s->grainsize) * s->cfg->sample_rate;
        float delay = param(float, s->graindelay) * s->cfg->sample_rate;
        float position = emod((int) (g->buffer->writetap.position - duration * speed), g->buffer->size);

        graintable_add_grain(g->grains,
            g->buffer,
            position,
            duration,
            delay,
            speed,
            param(int, s->eveloptype));
    }
    // post("graintable length: %d",graintable_get_len(g->grains));

    // update state of all grains
    graintable_update_lifetime(g->grains, n);

    // fetch grain to synthesize output
    gn = graintable_peek_grain(g->grains);
    if (gn && gn->lifetime > gn->delay){ // only relevant when grain delay is implemented
        graintable_pop_grain(g->grains);

        // Envelope 
        int attacksamples = param(float,s->attacktime)* s->cfg->sample_rate; //from time to samples
        int releasesamples = param(float,s->releasetime)* s->cfg->sample_rate;
        ep = evelopbuf_check_evelope(g->evelopes, gn->evelope, gn->gb_size,attacksamples,releasesamples);
        
        synthesizer_active_grain(g->synth, gn, ep);
    }

    synthesizer_write_output(g->synth, out, n);
}
