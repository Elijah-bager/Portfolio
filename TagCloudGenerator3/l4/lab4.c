// Copyright 2024 Neil Kirby 
// Not for disclosure without permission
//Edited by Elijah Baugher

// system libraries rarely have bugs, do them first
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// custom libraries and things like custom libraries go here
#include "n2.h"
#include "debug.h"
#include "mc.h"

// do C code headers last, this file.s header dead last.
#include "sim.h"
#include "linkedlist.h"

// my own header file is included dead last.  IT is MANDATORY!
#include "lab4.h"
#define TWO 2


/* I own all inits.  In future labs, I might morph to master init and call
 * my init minions to do the various subtasks.  For now, I'm simple enough
 * to do it all easily myself.  I shield main for all details about
 * initializations */
static bool init(int argc)
{
        if(argc<TWO)
        {
                printf("ERROR: insufficient args - no filename given\n");
                return false;
        }
        if(argc>TWO)
        {
                printf("ERROR: Bonus code is not present\n");
                return false;
        }

        // Our graphics library is a bit of a hog, so we only initialize it 
        return(TEXT || ( GRAPHICS && mc_initialize()));

}

/* Put all of the toys away.  In future labs I will own more stuff and
 * call my minions to do those things. */
void teardown()
{
        if(GRAPHICS)mc_teardown();
}

/* Avoid details and call upon my minions to make it everything happen.  I own
 * those highest level things that I must own: performance timing and the
 * value we return to linux */

#define RVAL_BAD_INIT (1)
#define RVAL_BAD_INPUT (2)





int main(int argc, char *argv[])
{
        
        int rval = EXIT_SUCCESS;
        double start, runtime;
        char *filename = argv[1];

        start = now();  // this is the very first executable statement

        if( init(argc))
        {
            if( !do_everything(filename)) rval = RVAL_BAD_INPUT;
            teardown();
        }
        else
        {
            rval = RVAL_BAD_INIT;       // non zero, indicates an error
        }

        /* at this point we are done, graphics has been torn down*/
        printf("Returning %d\n", rval);
        runtime = now() - start;
        /* after graphics has been torn down, we can freely print */
        printf("Total runtime is %.9lf seconds\n", runtime);

        return rval;
}

