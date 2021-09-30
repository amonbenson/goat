#include "graintable/graintable.h"
// #include "util/circbuf.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"

grain *grain_init(grain *gn, circbuf *cb, circbuf *pb, float position, float duration, float delay, float speed, size_t max_timeout, int evelope){
    gn->cb = cb;
    gn->pb = pb;

    gn->position = position;
    gn->duration = duration;
    gn->delay = delay;
    gn->speed = speed;
    gn->timeout = min(max_timeout, (size_t) (delay + duration / speed));
    gn->evelope  = evelope;
    gn->lifetime  = 0;

    // size of the internal grain buffer (used by the active grain and envelope buffer)
    gn->gb_size = min(cb->size, (size_t) (gn->duration * gn->speed + 1.0f));

    return gn;
}


void grain_post_feature(grain *gn){
    printf("features: \n position: %f | \t duration: %f | \t delay: %f | \t speed: %f | \t evelope: %d | \t lifetime: %" PRI_SIZE_T " | \t timeout %" PRI_SIZE_T "\n",
        gn->position,
        gn->duration,
        gn->delay,
        gn->speed,
        gn->evelope,
        gn->lifetime,
        gn->timeout);
}


void grain_update_lifetime(grain *gn, int n){
    gn->lifetime += n;
}


graintable *graintable_new(int size){
    graintable *gt = malloc(sizeof(graintable));
    if (!gt) return NULL;

    gt->data = malloc(sizeof(grain) * size);
    if (!gt->data) return NULL;

    gt->size = size;
    gt->front = 0;
    gt->rear = 0;

    return gt;
}


void graintable_free(graintable *gt){
    // free the buffer itself
    free(gt->data);
    free(gt);
}


int graintable_is_full(graintable *gt){
    return (gt->rear+1)%gt->size == gt->front?1:0;
}


int graintable_is_empty(graintable *gt){
    return gt->rear == gt->front?1:0;
}


void graintable_add_grain(graintable *gt, circbuf *cb, circbuf *pb, float position, float duration, float delay, float speed, int evelope){  
    if (graintable_is_full(gt) == 1){
        return;
    }

    // do not create grains with invalid durations
    float actualduration = duration / speed;
    if (actualduration < 2.0f || actualduration >= cb->size) {
        fprintf(stderr, "graintable: invalid grain duration: %f\n", actualduration);
        return;
    }

    size_t max_timeout = (size_t) (cb->size - delay - duration / speed);

    grain_init(&gt->data[gt->rear],
        cb,
        pb,
        position,
        duration,
        delay,
        speed,
        max_timeout,
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






