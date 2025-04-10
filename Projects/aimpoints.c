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


// do C code headers last, this file's header dead last.
#include "missiles.h"
#include "output.h"
#include "linkedlist.h"
// my own header file is included dead last.  IT is MANDATORY!
#include "aimpoints.h"

//Checks if any bases are present
bool have_bases(struct Sim *sim)
{
	for(int i = 0; i < BASE_COUNT; i++)
	{
	    if(sim->bases[i].color != 0)
	    {
		    return true;
	    }
	}
	return false;
}

//moves the aimpoint based on speed * time
static void stay_in_area(struct Aimpoint *aim)
{
	if(aim->ground_X >= FIELD_RIGHT)
	{
	    aim->ground_X = FIELD_RIGHT;
	    aim->ground_speed = - fabs(aim->ground_speed);
	    // move off the edge can't be on the edge
	    aim->ground_X  += aim->ground_speed * DELTA_T;
	}
	
	if(aim->ground_X <= FIELD_LEFT)
	{
	    aim->ground_X = FIELD_LEFT;
	    aim->ground_speed = fabs(aim->ground_speed);
	    // move off the edge can't be on the edge
	    aim->ground_X  += aim->ground_speed * DELTA_T;
	}
}
//updates aimpoint motion
static void basic_aimpoint_motion(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	double move = aim->ground_speed * DELTA_T;
	aim->ground_X  += move;
	stay_in_area(aim);
	if(DEBUG)printf("DEBUG: basic motion Base %d move is %.5lf, winds up at X=%.5lf\n", aim->color, move, aim->ground_X);
}
//checks to see if the position is correct
static void target_XYZ(struct Aimpoint *aim, struct Missile *mis, char *title)
{
	if(mis)
	{
		// update the aimpoint's position
		double available = fabs(aim->ground_speed * DELTA_T);
		double move = mis->x_position - aim->ground_X;
		// make sure that we move in the right direction
		aim->ground_speed = (move >=0) ? fabs(aim->ground_speed) : -fabs(aim->ground_speed);

		if (move > available) move = available;
		if (move < -available) move = -available;
		aim->ground_X += move;
		if(DEBUG)printf("DEBUG: %s Base %d moves %.5lf to X=%.5lf to get under %d (%.5lf,%.5lf).\n", 
			title, aim->color,move, aim->ground_X, mis->color, mis->x_position, mis->y_position);	
	}
	else
	{
			if(DEBUG)printf("DEBUG: %s Base %d has no target.\n", title, aim->color);	
	}
}

// I need some comparison functions for the linked list

// true if the second missile is lower than the first
bool lowest_missile(void *data1, void *data2)
{
	struct Missile *mis1 = data1;
	struct Missile *mis2 = data2;
	return mis1->y_position > mis2->y_position;
}	

// true if the second missile is to the left of the first
bool leftmost_missile(void *data1, void *data2)
{
	struct Missile *mis1 = data1;
	struct Missile *mis2 = data2;
	return mis1->x_position > mis2->x_position;
}	

// true if the second missile is to the right of the first
bool rightmost_missile(void *data1, void *data2)
{
	struct Missile *mis1 = data1;
	struct Missile *mis2 = data2;
	return mis1->x_position < mis2->x_position;
}	

static void brave(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	struct Missile *mis = best(sim->inflight, lowest_missile);
	target_XYZ(aim, mis, "Brave");	

}

static void afraid(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	struct Missile *mis = best(sim->inflight, lowest_missile);
	if(mis)
	{
		// update the aimpoint's position
		
		double diff = mis->x_position - aim->ground_X;
		// make sure that we move in the right direction
		aim->ground_speed = (diff <=0) ? fabs(aim->ground_speed) : -fabs(aim->ground_speed);

		if(DEBUG)printf("DEBUG: Afraid Base %d want to moves at speed %.5lf to get away from %d (%.5lf,%.5lf).\n", 
			aim->color, aim->ground_speed, mis->color, mis->x_position, mis->y_position);	
			
		basic_aimpoint_motion(aim, sim);

	}
	else
	{
			if(DEBUG)printf("DEBUG: Afraid Base %d has no target.\n", aim->color);	
	}
}

static void target_leftmost(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	struct Missile *mis = best(sim->inflight, leftmost_missile);
	target_XYZ(aim, mis, "Leftmost");	
}	

static void target_rightmost(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	struct Missile *mis = best(sim->inflight, rightmost_missile);
	target_XYZ(aim, mis, "Rightmost");
}	

static void stays_put(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	stay_in_area(aim);  // just making sure it's in the field
	if(DEBUG)printf("DEBUG: Base %d stays put at X=%.5lf.\n", aim->color, aim->ground_X);
}	

typedef void (*motion_function)(struct Aimpoint *aim, struct Sim *sim);

void aimpoint_motion(struct Aimpoint *aim, struct Sim *sim)
{
	// update the aimpoint's position
	motion_function regular_table[] = {
		basic_aimpoint_motion, basic_aimpoint_motion, 
		basic_aimpoint_motion, basic_aimpoint_motion,
		basic_aimpoint_motion, basic_aimpoint_motion, 		
		basic_aimpoint_motion, basic_aimpoint_motion
		};
		motion_function bonus_table[] = {
		basic_aimpoint_motion, brave, 
		afraid, target_leftmost,
		target_rightmost, stays_put, 		
		basic_aimpoint_motion, basic_aimpoint_motion
		};

	motion_function *jt = regular_table	;
	if(BONUS) jt = bonus_table;
	jt[aim->color](aim, sim);
}



//checks if missile is in beam path
bool in_beam(void *data, void *helper)
{
	struct Missile *mis = data;
	struct Aimpoint *aim = helper;

	double dx = fabs(mis->x_position - aim->ground_X);
	return dx <= aim->beam_width/ 2.0;
}
//shoot every second
static void maybe_shoot(struct Sim *sim, struct Aimpoint *aim)
{
	if( (int)sim->elapsed >  (int)(sim->elapsed - DELTA_T))
	{
		fire_message(aim);
		aim->score += deleteSome(&sim->inflight, in_beam, aim, destroy_missile, TEXT);
	}
}
//update aimpoint, maybe shoot
void update_an_aimpoint(struct Sim *sim, struct Aimpoint *aim)
{
	// update the aimpoint's position
	aimpoint_motion(aim, sim);
	maybe_shoot(sim, aim);
}

//loops through aimpoints to update
void update_aimpoints(struct Sim *sim)
{
	// TODO loop goes here
	for(int i = 0; i < BASE_COUNT; i++)
	{
	    if(sim->bases[i].color != 0)
	    {
		update_an_aimpoint(sim, &sim->bases[i]);
	    }
	}
}

