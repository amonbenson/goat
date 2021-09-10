#include "scheduler/scheduler.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"

scheduler *scheduler_new(void){
    scheduler *sd = malloc(sizeof(scheduler));
    if (!sd) return NULL;

    // configs from pd
    sd->streamsize = 64; 
    sd->samplerate = 44100;

    // basic user adjustable configs
    sd->gransize = 2048;  
    sd->interonset = 1024;
    sd->maxinteronset = 2048;  
    sd->mininteronset = 1024;  
    sd->eveloptype = 3;    

    sd->fetchgrain = 2048; // initial: gransize, at least enough of a single grain
    sd->synthgrain = 2048; // initial: max(mininteronset,gransize)

    // advance user adjustable configs 
    sd->getpitch = 0;       
    sd->getenergy = 0;      
    sd->pitch = 440.0;        
    sd->energy = 0.5;      
    sd->pitchratio = 2.0;   
    sd->energyratio = 0.99;  

    return sd;
}


void scheduler_free(scheduler *sd){
	free(sd);
}


int scheduler_get_next_interonset(int max, int min, int slot){
	srand((unsigned)time(NULL));
	int interonset = rand() % (max - min + 1) + max;
	return interonset - interonset % slot; // rond it to an integer multipule of streamsize
}


void scheduler_update_counter(scheduler *sd){

	if (sd->fetchgrain != 0){
		sd->fetchgrain = sd->fetchgrain - sd->streamsize;
		// post("sd->fetchgrain: %d", sd->fetchgrain);
	}
	else{
		sd->fetchgrain = sd->interonset - sd->streamsize;
		// post("sd->fetchgrain: %d", sd->fetchgrain);
	}

	if (sd->synthgrain != 0){
		sd->synthgrain = sd->synthgrain - sd->streamsize;
		// post("sd->synthgrain: %d", sd->synthgrain);
	}
	else{
		sd->synthgrain = scheduler_get_next_interonset(sd->maxinteronset, sd->mininteronset, sd->streamsize) - sd->streamsize;
		// post("sd->synthgrain: %d", sd->synthgrain);
	}
}


void scheduler_perform(scheduler *sd){
	// todo: add control function of other parameters
	scheduler_update_counter(sd);
}

