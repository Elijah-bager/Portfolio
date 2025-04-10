// Copyright 2025 Neil Kirby, not for dislosure without permission
//Edited by Elijah Baugher
//system libraries rarely have bugs, do them first
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// constants are next, along with structs
#include "constants.h"  
#include "field.h"  
#include "structs.h"
#include "debug.h"
#include "libll.h"
#include "linkedlist.h"


// do C code headers last, this file's header dead last.
#include "output.h"
#include "memory.h"
// my own header file is included dead last.  IT is MANDATORY!
#include "missiles.h"

#define BLAST_RADIUS 1.0

bool by_altitude(void *data1, void *data2)
{
	struct Missile *mis1 = data1, *mis2 = data2;
	return mis1->y_position > mis2->y_position;
}
//free missile from list
void destroy_missile(void *data)
{
	struct Missile *mis = data;
	dead_missile_message(mis);
	free_thing(mis);
}

//add missile to list
void add_missile(struct Sim *sim, struct Missile *mis)
{
	struct Missile *dynamic = allocate_thing(sizeof(struct Missile));
	if(dynamic)
	{
		*dynamic = *mis;
		dynamic->sim = sim;
		// consider placing these in the incoming list first
		if(!insert(&sim->inflight, dynamic, by_altitude, TEXT))
		{
			if(TEXT)printf("ERROR: failed to insert a missile\n");
			free_thing(dynamic);
		}
	}
	else
	{
		if(TEXT)printf("ERROR: failed to allocate memory for a missile\n");
	}
}
// missile motion
static void basic_motion(struct Missile *mis)
{
        // update the mis's position
        mis->x_position += mis->x_velocity * DELTA_T;
        mis->y_position += mis->y_velocity * DELTA_T;
}

// are we not yet at the altitude we want to detonate at?
static bool not_yet(struct Missile *mis)
{

	// the next few lines are a single statement
	return ( (mis->y_velocity < 0.0) ? 
		(mis->y_position > mis->det_alt)
		:
		(mis->y_position < mis->det_alt)
		)
		;
}
//check for in bounds
static bool in_bounds(struct Missile *mis)
{
	return(
	    (mis->x_position > FIELD_LEFT) &&   
	    (mis->x_position < FIELD_RIGHT) &&
	    (mis->y_position > FIELD_BOTTOM) &&   
	    (mis->y_position < FIELD_TOP) 
	);
}
//check for detonation
static bool boom_now(struct Missile *mis)
{
	return(
	    in_bounds(mis) &&
	    ( !not_yet(mis))
	    );
}

//checks if missile is inflight
bool missile_is_inflight(struct Missile *mis)
{
        return( 
		not_yet(mis)  &&
		in_bounds(mis)
        );
}
//checks if missile is not inflight
bool missile_not_inflight(void *data)
{
	struct Missile *mis = data;
	return !missile_is_inflight(mis);
}
//checks if bases are too close to missile detonation
bool too_close(struct Missile *mis, struct Aimpoint *base)
{
	double dx = mis->x_position - base->ground_X;
	double dy = mis->y_position ;	// bases sit at y== 0.0
	double distance = sqrt(dx * dx + dy * dy);
	return distance <= BLAST_RADIUS;
}
//destroy base if too close
void vaporize_bases(struct Missile *mis)
{
	for(int i=0; i<BASE_COUNT; i++)
	{
	    if(mis->sim->bases[i].color != 0)
	    {
		if(too_close(mis, &mis->sim->bases[i]))
		{
			// vaporize the base
			dead_base_message(&mis->sim->bases[i]);	
			mis->sim->bases[i].color = 0;
		}
	    }
	}	
}

//destroy missile from list
void purge_missiles(struct Sim *sim)
{
	int count;
	count = deleteSome(&sim->inflight, (CriteriaFunction) missile_not_inflight, NULL, free_thing, TEXT);
	if(DEBUG)
	{
if(count)printf("At time %.5lf, %d missiles were purged.\n", sim->elapsed, count);
	}
}
//update single missile
void update_a_missile(void *data)
{
	struct Missile *mis = data;
	struct Sim *sim = mis->sim;

	if(missile_is_inflight(mis))
	{
	    basic_motion(mis);
	    // can only boom if over the target area
	    if(boom_now(mis))
	    {
	    if(DEBUG)printf("Detonation at time %.5lf\n", sim->elapsed);
    detonation_message((int) mis->color, mis->x_position, mis->y_position);
			vaporize_bases(mis);
    	}

	    if(!in_bounds(mis))
	    {
    exit_area_message((int) mis->color, mis->x_position, mis->y_position);
	    }
	}
}
//iterate missiles to update
void update_missiles(struct Sim *sim)
{
	iterate(sim->inflight, update_a_missile);
}
