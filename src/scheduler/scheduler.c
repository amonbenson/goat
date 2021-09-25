#include "scheduler/scheduler.h"

#include <stdio.h>
#include "util/mem.h"
#include "util/util.h"

scheduler *scheduler_new(goat_config *cfg) {
    scheduler *sd = malloc(sizeof(scheduler));
    if (!sd) return NULL;

	sd->cfg = cfg;

    // basic user adjustable configs
<<<<<<< HEAD
    sd->grainsize = control_manager_parameter_add(cfg->mgr, "grainsize", 0.28, 0.1, 6.0); //values in seconds
    sd->graindist = control_manager_parameter_add(cfg->mgr, "graindist", -0.05, -1.0, 1.0);
	sd->graindelay = control_manager_parameter_add(cfg->mgr, "graindelay", 0.0, 0.0, 10.0);
    sd->eveloptype = control_manager_parameter_add(cfg->mgr, "envelope", 2, 0, 3);
	sd->attacktime = control_manager_parameter_add(cfg->mgr, "attacktime",0.12, 0, 0.4);
	sd->releasetime = control_manager_parameter_add(cfg->mgr, "releasetime",0.12, 0, 0.4);
=======
    sd->grainsize = control_manager_parameter_add(cfg->mgr, "grainsize", 0.01f, 0.0f, 6.0f); //values in seconds
    sd->graindist = control_manager_parameter_add(cfg->mgr, "graindist", -0.5f, -1.0f, 1.0f);
	sd->graindelay = control_manager_parameter_add(cfg->mgr, "graindelay", 0.0f, 0.0f, 10.0f);
	sd->grainpitch = control_manager_parameter_add(cfg->mgr, "grainpitch", 0.0f, -36.0f, 36.0f);
    sd->eveloptype = control_manager_parameter_add(cfg->mgr, "grainenv", 3, 0, 3);

>>>>>>> f6dcf6374a214dc0630b58ab788b863424cec8d5
    sd->lastfetch = 0;
	sd->dofetch = 0;

    return sd;
}


void scheduler_free(scheduler *sd){
	control_manager_parameter_remove(sd->cfg->mgr, sd->grainsize);
	control_manager_parameter_remove(sd->cfg->mgr, sd->graindist);
	control_manager_parameter_remove(sd->cfg->mgr, sd->graindelay);
	control_manager_parameter_remove(sd->cfg->mgr, sd->grainpitch);
	control_manager_parameter_remove(sd->cfg->mgr, sd->eveloptype);

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
	// calculate the distance in samples between two grains.
	float actualduration = param(float, sd->grainsize) * sd->cfg->sample_rate * semitonefact(param(float, sd->grainpitch));
	size_t nextfetch = actualduration * (1.0f + param(float, sd->graindist));

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

