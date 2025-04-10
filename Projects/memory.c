// Copyright 2025 Neil Kirby, not for dislosure without permission
//Edited by Elijah Baugher
//system libraries rarely have bugs, do them first
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//constants

// do C code headers last, this file's header dead last.
#include "debug.h"
#include "libll.h"
#include "altmem.h"
// my own header file is included dead last.  IT is MANDATORY!
#include "memory.h"




//allocate using alternative_malloc
void *allocate_thing(long size)
{
    static int count = 0;
    void *thing = alternative_malloc(size);
    if(thing)
    {
        count++;
        if(TEXT)
        {
            printf("DIAGNOSTIC allocated %ld bytes at %p, %d things allocated\n", size, thing, count);
        }
    }
    else
    {
        if(TEXT)printf( "ERROR: failed to allocate %ld bytes\n", size);
    }
    return thing;
}
//free using alternative_free
void free_thing(void *thing)
{
    static int count = 0;
    if(thing)
    {
        if(DEBUG)
	{
	    printf("DEBUG: freeing memory at %p\n", thing);
	    fflush(stdout);
	}
         alternative_free(thing);
         count++;
        if(TEXT) printf("DIAGNOSTIC %d things freed\n", count);       
    }
    else
    {
        if(TEXT)printf( "ERROR: attempt to free NULL\n");
    }
}
