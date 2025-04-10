//Edited by Elijah Baugher
#include <stdbool.h>
#include <stdio.h>

// custom libraries and things like custom libraries go here   
#include "structs.h" 

//  do C code headers last, this file.s header dead last.
#include "output.h"
#include "bits.h"
#include "missiles.h"
#include "linkedlist.h"
#include "debug.h"

// my own header file is included dead last.  IT is MANDATORY!
#include "input.h"

//reads aimpoint data
static bool read_an_aimpoint(struct Sim *sim, FILE *file)
{
	unsigned int bits;
	struct Aimpoint sky = {0}, *aim = &sky;
	int tokens;

//TODO add the other data items - AI - to this

	if(4==(
		tokens =fscanf(file,"%x %lf %lf %lf", &bits,  
			&aim->beam_width, &aim->ground_speed, &aim->ground_X)
		)
	    )
	{
	    if(!valid_bits(bits))
	    {
	    	bad_bits_message("aim", bits);
		return false;
	    }
	    aim->color = color_from_bits(bits);
		// put it in the sim
		sim->bases[aim->color] = *aim;
	}
	else
	{
	    scanf_message("aimpoint",  tokens, 4);
	    return false;
	}


	return true;
}
//reads in aimpoints
static bool read_aimpoints(struct Sim *sim,FILE *file)
{
    //only the last one counts
    int count, tokens;
    if(1==(tokens = fscanf(file,"%d", &count)))
    {
        for(int i = 0; i < count; i++)
        {
	    if(!read_an_aimpoint(sim,file))return false;
        }
    }
    else
    {
        scanf_message("aimpoint count", tokens, 1);
        return false;
    }
    return true;
}
//read missile data
static bool read_a_missile(struct Sim *sim,FILE *file)
{
	struct Missile sky = {0}, *mis = &sky;
	int tokens;
	unsigned int bits;

	if(6==(tokens =fscanf(file,"%x %lf %lf %lf %lf %lf", &bits, 
		    &mis->x_position, &mis->y_position, &mis->x_velocity, 
		    &mis->y_velocity, &mis->det_alt)
		)
	    )
	{
	    if(!valid_bits(bits))
	    {
	    	bad_bits_message("missile", bits);
		return false;
	    }
		mis->color = color_from_bits(bits);
	    //we could validate here
		add_missile(sim, mis);
	}
	else
	{
	    scanf_message("misile",  tokens, 6);
	    return false;
	}
	return true;
}
//reads in missiles 
static bool read_missiles(struct Sim *sim, FILE *file)
{
    //only the last one counts
    int count, tokens;
    if(1==(tokens = fscanf(file,"%d", &count)))
    {
        for(int i = 0; i < count; i++)
        {
	    if(!read_a_missile(sim,file))return false;
	}
    }
    else
    {
        scanf_message("misile count", tokens, 1);
        return false;
    }

    return true;
}

//Opens input file
static FILE *open_file(const char *file)
{
        FILE *fp=fopen(file,"r");
        if(fp!=NULL)
        {
              if(TEXT)  printf("DIAGNOSTIC: Successfully opened %s for reading.\n",file);
              return fp;
              }
        else
        {
              if(TEXT)  printf("ERROR: unable to open %s for reading.\n ", file);
              return false;
        }
        
        
}

//close input file
static void close_file(FILE *file)
{
        FILE *fp=file;
        if(fclose(fp)==0)
        {
                if(TEXT) printf("DIAGNOSTIC: Input file closed\n");
        }
        else
        {
                if(TEXT) printf("ERROR: failed to close input file\n");
        }
}
//Checks for valid input, then populates data
bool good_input( struct Sim *sim, char *filename)
{
	FILE *file;
	if(!(file=open_file(filename)))
	{
		close_file(file);
		return false;
	}

    if(!read_missiles(sim,file))return false;
    if(!read_aimpoints(sim,file))return false;
	close_file(file);
    return true;
}

