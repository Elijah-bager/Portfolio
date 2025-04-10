// Copyringht  2025 Neil Kirby not for disclosure wiothout permission
//Edited by Elijah Baugher

// system libraries rarely have bugs, do them first
#include <stdbool.h>
#include <stdio.h>  
// custom libraries are usxually clean as well
#include "mc.h"
// constants are next, along with structs
#include "constants.h"
#include "debug.h"
#include "structs.h"

// do C code headers last, this file's header dead last.
#include "n2.h"
#include "text.h"
#include "graphics.h"
#include "linkedlist.h"

// my own header file is included dead last.  IT is MANDATORY!  
#include  "output.h"



static bool print_now(double elapsed)
{
        if(DEBUG) return true;
        return(elapsed == 0.0 || (int) elapsed > (int) (elapsed - DELTA_T));
}

#define SLOW 4.0
#define NORMAL_SPEED 1.0
void master_output(struct Sim *sim)
{
    if(TEXT)
    {
        if(print_now(sim->elapsed))master_print(sim);
    }
    //if(GRAPHICS)master_draw(sim, SLOW);
    if(GRAPHICS)master_draw(sim, NORMAL_SPEED);
}


void final_output(struct Sim *sim)
{
    if(TEXT)master_print(sim);
    if(GRAPHICS)freeze(sim);
}

// various mesages
void fire_message(struct Aimpoint *base)
{
    if(TEXT)printf("Base %7s fires a %0.2lf wide beam at X=%.5lf.\n", number_to_color(base->color), base->beam_width, base->ground_X);
    if(GRAPHICS)
    {
        mc_beam(base->color, base->ground_X, base->beam_width);
    }
}

void dead_base_message(struct Aimpoint *base)
{
    if(TEXT)printf("Base %7s was destroyed at X=%.5lf.\n", number_to_color(base->color), base->ground_X);
    if(GRAPHICS)
    {
        char buf[80];
        sprintf(buf, "Base %7s was destroyed at X=%.5lf.\n", number_to_color(base->color), base->ground_X);
        mc_status(buf);
    }
}

void dead_missile_message(struct Missile *mis)
{
    if(TEXT)printf("Missile %7s was destroyed at (%.5lf, %.5lf).\n", number_to_color(mis->color), mis->x_position, mis->y_position);
    if(GRAPHICS)
    {
        char buf[80];
        sprintf(buf, "Missile %7s was destroyed at (%.5lf, %.5lf).\n", number_to_color(mis->color), mis->x_position, mis->y_position);
        mc_status(buf);
    }
}

void exit_area_message(int color, double X, double Y)
{
    if(TEXT)printf("Missile %s exits target area to (%.5lf, %52lf).\n", number_to_color(color), X, Y);
    if(GRAPHICS)
    {
        char buf[80];
sprintf(buf, "Missile %s exits target area to (%.5lf, %.5lf).", number_to_color(color), X, Y);
        mc_status(buf);
    }
}

void detonation_message(int color, double X, double Y)
{
    if(TEXT)printf("Missile %s detonates at (%.2lf, %.2lf).\n", number_to_color(color), X, Y);
    if(GRAPHICS)
    {
        char buf[80];
        sprintf(buf, "Missile %s detonates at (%.2lf, %.2lf).", number_to_color(color), X, Y);
        mc_status(buf);
	mc_boom(color, X, Y);
    }
}

void scanf_message(char *who, int got, int wanted)
{
    if(TEXT)printf("ERROR: %s read %d of %d tokens.\n", who, got, wanted);
}

void bad_bits_message( char *where, unsigned int bits)
{
    if(TEXT)printf("ERROR: Bad %s bits (%X) detected.\n", where, bits );
}

void bad_missile_message(struct Missile *mis)
{
    if(TEXT)printf("ERROR: Bad missile detected.\n" );
}

