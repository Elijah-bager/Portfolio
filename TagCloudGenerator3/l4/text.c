// Copyringht  2025 Neil Kirby not for disclosure wiothout permission
//Edited by Elijah Baugher
// system libraries rarely have bugs, do them first
#include <stdbool.h>
#include <stdio.h>  
// custom libraries are usxually clean as well
// THis file only does text output, so it is ok

// constants are next, along with structs
#include "constants.h"
#include "debug.h"
#include "structs.h"
#include "libll.h"

// do C code headers last, this file's header dead last.
#include "missiles.h"
#include "aimpoints.h"
#include "linkedlist.h"

// my own header file is included dead last.  IT is MANDATORY!  
#include  "text.h"

//Function that takes a color as a number and returns a string
char * number_to_color(int color)
{
 char *arr[]= {"Black","Red","Green","Yellow","Blue","Magenta","Cyan","White"};
 return arr[color];
}

static void print_a_missile(void *data)
{
		struct Missile *mis = data;
		if(missile_is_inflight(mis))
		{
printf("%7s missile is at (%8.5lf, %8.5lf) with velocity (%8.5lf, %8.5lf).\n", number_to_color(mis->color), mis->x_position, mis->y_position, mis->x_velocity, mis->y_velocity);
    	}
}
//print missiles in list
static void print_missiles(struct Sim *sim)
{
	if(sim->inflight)
	{
		printf("Missiles in flight:\n");
		iterate(sim->inflight, print_a_missile);
	}
	else
	{
		printf("No missiles remain.\n");	
	}
}	
//print aimpoints in array
static void print_aimpoints(struct Sim *sim)
{
	if(have_bases(sim))
	{
		printf("Bases:\n");
		for(int i = 0; i < BASE_COUNT; i++)
		{
			if(sim->bases[i].color != 0)
			{
	printf("%7s base is at  %8.5lf moving %8.5lf.\n", number_to_color(sim->bases[i].color), 
				sim->bases[i].ground_X, sim->bases[i].ground_speed);
			}
		}
	}
	else
	{
		printf("No bases remain.\n");
	}	

}




//calls all print functions
void master_print(struct Sim *sim)
{
	printf("\nElapsed time: %8.5lf\n", sim->elapsed);
	print_missiles(sim);
	print_aimpoints(sim);
	printf("\n");
}



