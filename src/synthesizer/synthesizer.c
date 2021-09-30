#include "synthesizer/synthesizer.h"


#include <stdio.h>
#include <math.h>
#include "util/mem.h"
#include "util/util.h"


#define SYNTH_MIN_SPEED 0.001f
#define SYNTH_MAX_SPEED 1000.0f


activategrain *activategrain_new(grain* gn, evelope* ep, int repeat, int relativepitch){
    float pitch, pitch_median, pitch_sum;
    float speed;

    activategrain *ag = malloc(sizeof(activategrain));
    if (!ag) return NULL;

    memcpy(&ag->origin, gn, sizeof(grain));

    ag->data = malloc(sizeof(float) * gn->gb_size); // to store grain samples
    if (!ag->data) return NULL;

    int bufstart = emod((int) (gn->position - gn->delay), gn->cb->size);

    // determine the speed of the grain
    if (relativepitch) {
        // get the median pitch
        gn->pb->readtaps->position = bufstart;

        pitch_median = 0.0f;
        pitch_sum = 0.0f;
        for (size_t i = 0; i < gn->gb_size; i++) {
            pitch = circbuf_read_interp(gn->pb, 0);
            if (pitch == -1) continue; // ignore unvoiced samples

            pitch_median += pitch;
            pitch_sum++;
        }

        if (pitch_sum == 0) {
            // no pitch information available
            speed = 1.0f;
        } else {
            pitch_median /= pitch_sum;
            speed = 220.0f / pitch_median * gn->speed;
        }
    } else {
        speed = gn->speed;
    }

    // read the data block with the specific speed
    gn->cb->readtaps->position = bufstart;
    gn->cb->readtaps->speed = speed;
    circbuf_read_block(gn->cb, 0, ag->data, gn->gb_size);

    // apply the grain envelope
    for (size_t i = 0; i < gn->gb_size; i++) {
        ag->data[i] *= ep->data[i];
    }

    ag->pos = 0;
    ag->length = gn->gb_size;
    ag->repeat = repeat;

    return ag;
}


void activategrain_free(activategrain *ag){
    if (!ag) {
        // post("grain already freed!");
        return; 
    }
    free(ag->data);
    free(ag);
    // post("activategrain freed!");
}


synthesizer *synthesizer_new(int length){
    synthesizer *syn = malloc(sizeof(synthesizer));
    if (!syn) return NULL;

    syn->data = malloc(sizeof(p_activategrain) * length);
    if (!syn->data) return NULL;

    for (int i = 0; i < length; i++){
        syn->data[i] = NULL;
    }
    syn->length = length;

    return syn;
}


void synthesizer_free(synthesizer *syn){
    for (int i = 0; i < syn->length; i++){
        activategrain_free(syn->data[i]);
    }
    free(syn->data);
    free(syn);
    // post("synthesizer freed!");
}


void synthesizer_active_grain(synthesizer *syn, grain* gn, evelope* ep, int relativepitch){
    activategrain *ag = activategrain_new(gn, ep, 0, relativepitch); // set repeat to 0
    if (!ag) return;

    for (int i = 0; i < syn->length; i++){
        if (syn->data[i] == NULL){
            syn->data[i] = ag;
            return;
        }
    }
    // post("synthesizer_active_grain: no more space for another activated grain, discard");
}


void synthesizer_freeze_grains(synthesizer *syn, int repeat){
    activategrain *ag = NULL;
    for (int i = 0; i < syn->length; i++){
        if (syn->data[i] != NULL){
            ag = syn->data[i];
            ag->repeat = repeat;
        }
    }
}


float synthesizer_sum_samples(synthesizer *syn){
    float tmp = 0.0;
    activategrain *ag = NULL;
    for (int i = 0; i < syn->length; i++){
        if (syn->data[i] != NULL){
            ag = syn->data[i];
            tmp += ag->data[ag->pos];
            ag->pos++;
            if (ag->pos >= ag->length && ag->repeat == 0){
                activategrain_free(ag); 
                syn->data[i] = NULL;
            }
            else if (ag->repeat == 1){
                ag->pos = 0;
            }
        }
    }

    return tmp;
}


void synthesizer_write_output(synthesizer *syn, float *out, int n){
    while (n--) *out++ = synthesizer_sum_samples(syn);
}








