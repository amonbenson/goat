#include "scheduler/scheduler.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"

scheduler *scheduler_new(goat_config *cfg) {
    scheduler *sd = malloc(sizeof(scheduler));
    if (!sd) return NULL;

	sd->cfg = cfg;

    // basic user adjustable configs
    sd->grainsize = control_manager_parameter_add(cfg->mgr, "grainsize", 0.1, 0.1, 6.0); //values in seconds
    sd->graindist = control_manager_parameter_add(cfg->mgr, "graindist", 0.0, -1.0, 1.0);
	sd->graindelay = control_manager_parameter_add(cfg->mgr, "graindelay", 0.0, 0.0, 10.0);
    sd->eveloptype = 3;

    sd->lastfetch = 0;
	sd->dofetch = 1; // do an initial fetch

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


/* void scheduler_update_counter(scheduler *sd, int n){

	if (sd->fetchgrain != 0){
		sd->fetchgrain = sd->fetchgrain - n;
		// post("sd->fetchgrain: %d", sd->fetchgrain);
	}
	else{
		sd->fetchgrain = sd->interonset - n;
		// post("sd->fetchgrain: %d", sd->fetchgrain);
	}

	if (sd->synthgrain != 0){
		sd->synthgrain = sd->synthgrain - n;
		// post("sd->synthgrain: %d", sd->synthgrain);
	}
	else{
		sd->synthgrain = scheduler_get_next_interonset(sd->maxinteronset, sd->mininteronset, n) - n;
		// post("sd->synthgrain: %d", sd->synthgrain);
	}
} */


void scheduler_perform(scheduler *sd, int n){
	// todo: add control function of other parameters
	// scheduler_update_counter(sd, n);

	// calculate the distance in samples between two grains.
	int nextfetch = param(float, sd->grainsize) * sd->cfg->sample_rate * (1.0f + param(float, sd->graindist));

	// if enough time elapsed, mark the grain as ready to be fetched and reset the lastfetch counter
	if (sd->lastfetch > nextfetch) {
		sd->dofetch = 1;
		sd->lastfetch = 0;
	} else {
		sd->dofetch = 0;
	}

	// update the lastfetch counter
	sd->lastfetch += n;
}

