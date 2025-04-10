// Copyringht  2025 Neil Kirby not for disclosure wiothout permission

// system libraries rarely have bugs, do them first
#include <stdbool.h>
#include <stdio.h>  
// custom libraries are usxually clean as well
#include "mc.h"
#include "libll.h"
// constants are next, along with structs
#include "constants.h"
#include "debug.h"
#include "structs.h"

// do C code headers last, this file's header dead last.
#include "n2.h"
#include "missiles.h"
#include "linkedlist.h"

// my own header file is included dead last.  IT is MANDATORY!  
#include  "graphics.h"

static void draw_aimpoints(struct Sim *sim)
{
    for(int i = 0; i < BASE_COUNT; i++)
    {
        if(sim->bases[i].color != 0)
        {
            mc_base((int) sim->bases[i].color, sim->bases[i].score, sim->bases[i].ground_X);
        }
    }
}


static void draw_missiles(void *data)
{
        struct Missile *mis = data;

        if(missile_is_inflight(mis))
        {
        mc_missile((int) mis->color, mis->x_position, mis->y_position);
        }
}


void master_draw(struct Sim *sim, double slowdown)
{
        mc_clear();
        mc_sim_time((int) (sim->elapsed * 1000));

        draw_aimpoints(sim);

	iterate(sim->inflight, draw_missiles);
        mc_refresh();
        microsleep((int) (slowdown * DELTA_T * 1000000));  
}


#define NORMAL_SPEED (1.0)
void freeze(struct Sim *sim)
{
    for(double d = 0.0; d < 4.0; d += DELTA_T)
    {
        master_draw(sim, NORMAL_SPEED);
    }
}

