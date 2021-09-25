#include "synthesizer/synthesizer.h"


#include <stdio.h>
#include <math.h>
#include "util/mem.h"
#include "util/util.h"

activategrain *activategrain_new(grain* gn, evelope* ep, int repeat){
    size_t actualduration = gn->duration * gn->speed;

    // samples with invalid length will cause issues and crashes, so we sort them out
    if (actualduration > gn->buffer->size || actualduration < 2.0f) {
        return NULL;
    }

    activategrain *ag = malloc(sizeof(activategrain));
    if (!ag) return NULL;

    ag->data = malloc(sizeof(float) * (size_t) (actualduration + 1.0f)); // to store grain samples
    if (!ag->data) return NULL;

    gn->buffer->readtaps->position = emod((int) (gn->position - gn->delay), gn->buffer->size);
    gn->buffer->readtaps->speed = gn->speed;
    circbuf_read_block(gn->buffer, 0, ag->data, actualduration); //can only use the first readtap

    for (size_t i = 0; i < actualduration; i++){
        ag->data[i] = ag->data[i] * ep->data[i]; // multiply the evelope
    }

    ag->pos = 0;
    ag->length = actualduration;
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


void synthesizer_active_grain(synthesizer *syn, grain* gn, evelope* ep){
    activategrain *ag = activategrain_new(gn, ep, 0); // set repeat to 0
    if (!ag) return;

    for (int i = 0; i < syn->length; i++){
        if (syn->data[i] == NULL){
            syn->data[i] = ag;
            return;
        }
    }
    post("synthesizer_active_grain: no more space for another activated grain, discard");
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
        }
    }

    return tmp;
}


void synthesizer_write_output(synthesizer *syn, float *out, int n){
    while (n--) *out++ = synthesizer_sum_samples(syn);
}








