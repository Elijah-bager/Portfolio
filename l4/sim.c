// Copyright 2025 Neil Kirby, not for dislosure without permission
//Edited by Elijah Baugher
//system libraries rarely have bugs, do them first
#include <stdbool.h>
#include <stdio.h>

// constants are next, along with structs
#include "constants.h"  
#include "structs.h"
#include "debug.h"

#include  "libll.h"

// do C code headers last, this file.s header dead last.
#include "input.h"
#include "output.h"
#include "missiles.h"
#include "aimpoints.h"
#include "memory.h"
#include "linkedlist.h"

// my own header file is included dead last.  IT is MANDATORY!
#include "sim.h"
//update missiles,aimpoints
static void update_everything(struct Sim *sim)
{
        update_missiles(sim);
        purge_missiles(sim);  // detonated or out of bounds
        update_aimpoints(sim);
}

//if we have bases and missile is inflight, game is on
static bool game_on(struct Sim *sim)
{
        //play if we are alive and there are still missiles around
        return( 
            ( sim->inflight) &&
            have_bases(sim)
        );        
}
//criteria function
bool always_true(void *data, void *helper)
{
    return true;
}
//free missiles from list
void free_stuff(void **p2listHead, char *name)
{
    int count = 0;
    
    count = deleteSome(p2listHead, always_true, NULL, free_thing, TEXT);
    if(TEXT)printf("Freed %d %s\n", count, name);
}
//free all missiles
static void free_everything(struct Sim *sim)
{
        if(TEXT)printf("Freeing everything\n");
        free_stuff(&sim->inflight, "inflight missiles");
}

//run sim 
static void run_simulation(struct Sim *sim)
{
        // run the simulation until we run out of either missiles or bases
        while(game_on(sim))
        {
            sort(sim->inflight, by_altitude);
            master_output(sim);
            // upoate the simulation, starting with the clock
            sim->elapsed += DELTA_T;
	    if(DEBUG)printf("DEBUG: Clock moved to %.5lf\n", sim->elapsed);
            update_everything(sim);            
        }
        final_output(sim);
        free_everything(sim);
}
//run sim with given input file to check good input
bool do_everything(char *filename)
{
            // we own the input data, so we have to declare it here
    struct Sim earth = {0.0}, *sim = &earth;



        if(good_input(sim,filename))
        { 
            run_simulation(sim);
        }
        else
        {
            return false;
        }
        return true;
}

