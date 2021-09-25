#include "graintable/graintable.h"
// #include "util/circbuf.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"

grain *grain_init(grain *gn, circbuf *cb, int position, int duration, int delay, int timeout, int evelope){
    gn->buffer = cb;

    gn->position = position;
    gn->duration = duration;
    gn->delay = delay;
    gn->evelope  = evelope;
    gn->timeout = timeout;
    gn->lifetime  = 0;

    gn->frequency = -1.0;
    gn->energy = -1.0;

    return gn;
}


void grain_post_feature(grain *gn){
    post("features: \n position: %d | \t duration: %d | \t evelope: %d | \t lifetime: %d | \t frequency: %f | \t energy: %f \n",
        gn->position,
        gn->duration,
        gn->evelope,
        gn->lifetime,
        gn->frequency,
        gn->energy
        );
}


void grain_update_lifetime(grain *gn, int n){
    gn->lifetime += n;
}


void grain_update_feature(grain *gn, float value, int feature){
    switch(feature){
        case 0: //frequency
            gn->frequency = value;
            break;
        case 1: //energy
            gn->energy = value;
            break;
        default:
            fprintf(stderr, "grain_update_feature: unsupported feature type, please check it again!");
    }
}




graintable *graintable_new(int size){
    graintable *gt = malloc(sizeof(graintable));
    if (!gt) return NULL;

    gt->data = malloc(sizeof(grain) * size);
    gt->size = size;
    gt->front = 0;
    gt->rear = 0;
    // post("graintable newed!");
    return gt;
}


void graintable_free(graintable *gt){
    // free the buffer itself
    free(gt->data);
    free(gt);
    // post("graintable freed!");
}


int graintable_is_full(graintable *gt){
    return (gt->rear+1)%gt->size == gt->front?1:0;
}


int graintable_is_empty(graintable *gt){
    return gt->rear == gt->front?1:0;
}


void graintable_add_grain(graintable *gt, circbuf *cb, int position, int duration, int delay, int evelope){  
    if (graintable_is_full(gt) == 1){
        return;
    }

    int timeout = cb->size - duration - delay;

    grain_init(&gt->data[gt->rear],
        cb,
        position,
        duration,
        delay,
        timeout,
        evelope);       
    gt->rear = (gt->rear+ 1) % gt->size;    
}


grain *graintable_peek_grain(graintable *gt){
    if (graintable_is_empty(gt) == 1){
        return NULL;
    }

    return &gt->data[gt->front];
}


grain *graintable_pop_grain(graintable *gt){
    grain *gn;

    if (graintable_is_empty(gt) == 1){
        return NULL;
    }
    gn = &gt->data[gt->front]; 

    // grain_post_feature(gn);
    gt->front = (gt->front + 1) % gt->size;
    return gn;
}


int graintable_get_len(graintable *gt){
    return (gt->rear - gt->front + gt->size)%gt->size;
}


void graintable_check_grain(graintable *gt, __attribute__((unused)) grain *gn, int delay){
    if (graintable_get_len(gt) <=  delay || delay < 0){
        error("graintable_check_grain: trying to check a grain out of current table range\n");
        return;
    }
}


void graintable_update_lifetime(graintable *gt, int n){
    for (int i = 0; i < graintable_get_len(gt); i++){
        grain_update_lifetime(&gt->data[(gt->front+i) % gt->size], n);
    }
}


void graintable_print_all(graintable *gt){
    for (int i = 0; i < graintable_get_len(gt); i++){
        grain_post_feature(&gt->data[(gt->front+i) % gt->size]);
    }
}






