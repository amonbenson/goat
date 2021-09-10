#include "graintable/graintable.h"
// #include "util/circbuf.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"

grain *grain_new(circbuf *cb, int position, int duration, int timeout, int evelope){
    grain *gn = malloc(sizeof(grain));
    if (!gn) return NULL;

    gn->buffer = cb;

    gn->position = position;
    gn->duration = duration;
    gn->evelope  = evelope;
    gn->timeout  = timeout;

    gn->frequency = -1.0;
    gn->energy = -1.0;

    return gn;
}


void grain_free(grain *gn){
    // post("grain freed!");
    free(gn);
}


void grain_post_feature(grain *gn){
    post("features: \n position: %d | \t duration: %d | \t evelope: %d | \t timeout: %d | \t frequency: %f | \t energy: %f \n",
        gn->position,
        gn->duration,
        gn->evelope,
        gn->timeout,
        gn->frequency,
        gn->energy
        );
}


void grain_update_timeout(grain *gn, int n){
    gn->timeout = gn->timeout - n;
}


void grain_update_feature(grain *gn, float value, int feature){
    switch(feature){
        case 0: //frequency
            gn->frequency = value;
        case 1: //energy
            gn->energy = value;
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
    // free every grain in the table
    grain* gn = NULL; // tmp grain saver
    while (graintable_is_empty(gt) == 0){
        gn = graintable_pop_grain(gt, gn);   
        grain_free(gn);
    }

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


void graintable_add_grain(graintable *gt, circbuf *cb, int grainsize, int evelope){  
    if (graintable_is_full(gt) == 1){
        return;
    }
    int position, timeout;
    grain *gn;

    position = ((int)cb->writetap.position - grainsize) % cb->size;
    timeout  = cb->size - grainsize;

    gn = grain_new(cb, position, grainsize, timeout, evelope);
    gt->data[gt->rear]= gn;               
    gt->rear = (gt->rear+ 1) % gt->size;    
}


grain *graintable_pop_grain(graintable *gt, grain *gn){
    if (graintable_is_empty(gt) == 1){
        return NULL;
    }
    gn = gt->data[gt->front]; 
      
    gt->data[gt->front] = NULL;
    // grain_post_feature(gn);
    gt->front = (gt->front + 1) % gt->size;
    return gn;
}


int graintable_get_len(graintable *gt){
    return (gt->rear - gt->front + gt->size)%gt->size;
}


void graintable_check_grain(graintable *gt, grain *gn, int delay){
    if (graintable_get_len(gt) <=  delay || delay < 0){
        error("graintable_check_grain: trying to check a grain out of current table range\n");
        return;
    }
    gn = gt->data[gt->front+delay];
}


void graintable_update_timeout(graintable *gt, int n){
    for (int i = 0; i < graintable_get_len(gt); i++){
        grain_update_timeout(gt->data[(gt->front+i) % gt->size], n);
    }
}


void graintable_print_all(graintable *gt){
    for (int i = 0; i < graintable_get_len(gt); i++){
        grain_post_feature(gt->data[(gt->front+i) % gt->size]);
    }
}






