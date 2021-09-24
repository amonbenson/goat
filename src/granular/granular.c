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


void granular_synthesize(granular *g, int blockstart, float *out, int n) {
    // alternate synthesis method
    grain *gn;
    int i, j;
    size_t position;
    size_t readtapid;
    size_t grainstart, grainend;
    circbuf_readtap *readtap;

    // loop through each sample
    for (i = 0; i < n; i++) {
        position = blockstart + i;
        readtapid = 0;
        out[i] = 0.0f;

        // loop through each grain
        for (j = 0; j < graintable_get_len(g->grains); j++){
            gn = &g->grains->data[(g->grains->front + j) % g->grains->size];
            grainstart = gn->position;
            grainend = (grainstart + gn->duration) % g->buffer->size;

            // if the grain is not active, skip it
            if (!CIRCBUF_INRANGE(grainstart, grainend, position)) continue;

            // setup the readtap and read the sample
            readtap = &g->buffer->readtaps[readtapid];
            readtap->position = grainstart + i;
            readtap->speed = 1.0f;

            /* printf("writing at %lu, reading tap %lu at %f --- %d\n", 
                position,
                readtapid,
                readtap->position,
                gn->position); */
            out[i] += circbuf_read_interp(g->buffer, readtapid);

            // only continue if we have readtaps left
            readtapid++;
            if (readtapid >= g->buffer->num_readtaps) break;
        }
    }
}


void granular_perform(granular *g, scheduler *s, float *in, float *out, int n) {
    grain *gn;
    size_t blockstart = g->buffer->writetap.position;

    // Delayline load input stream
    circbuf_write_block(g->buffer, in, n); //load input stream into circbuf constantly @todo add parameter to stop and continue loading 
    
    // sample new grain and add into the graintable
    if (s->dofetch){
        graintable_add_grain(g->grains,
            g->buffer,
            blockstart,
            param(float, s->grainsize) * s->cfg->sample_rate,
            0);
    }

    // remove old grains
    gn = graintable_peek_grain(g->grains);
    if (gn && (gn->lifetime >= gn->duration || (size_t) gn->lifetime >= g->buffer->size)) {
        graintable_pop_grain(g->grains);
    }

    // update state of all grains
    graintable_update_lifetime(g->grains, n);

    // synthesize grains
    granular_synthesize(g, blockstart, out, n);

    // fetch grain to synthesis output
    /* if (s->synthgrain == 0) {
        // gn = graintable_check_grain(g->grains, gn, 0); // set delay to 0 is actully fetch the same grain except won't move it out
        do{
            gn = graintable_pop_grain(g->grains, gn);
        }while (gn->timeout <= 0);  

        ep = evelopbuf_check_evelope(g->evelopes, gn->evelope, gn->duration);
        synthesizer_active_grain(g->synth, gn, ep);
    }

    synthesizer_write_output(g->synth, out, n); */
}
