// Copyright 2025 Neil Kirby, not for dislosure without permission
//edited by Elijah Baugher
//system libraries rarely have bugs, do them first
#include <stdbool.h>
#include <stdio.h>

// constants are next, along with structs
#include "debug.h"

// do C code headers last, this file.s header dead last.

// my own header file is included dead last.  IT is MANDATORY!
#include "bits.h"


// personal defines:
#define COLOR_SHIFT 4
#define COLOR_MASK 0x7

#define HI_BIT (1<<31)
//validate bits
bool valid_bits(unsigned int bits)
{
	int count = 0;
	int space = 0;
	bool rval;
	int this;
	
	if(DEBUG)printf("DEBUG: valid bits %X\n   ", bits);
	for(int i=0; i<32; i++)
	{
	    this = ((bits & HI_BIT) != 0);
	    count += this;
	    if(DEBUG)
	    {
		if(space % 4 == 0)printf(" ");
		printf("%d", this);
	    }
	    bits = bits << 1;
	    space++;
	}
	if(DEBUG)printf("\n    That is %d one bits\n", count);

	rval = count & 1; // odd is true
	if(DEBUG)printf("    returning %d\n", rval);


	return rval;
}
//extract color
static int extract(unsigned int bits, int shift, int mask)
{
	int rv = bits >> shift;
	if(DEBUG) printf("    shifting by %d giving %X\n", shift, rv);
	rv = rv & mask;
	if(DEBUG) printf("    masking with %X giving %X\n", mask, rv);
	if(DEBUG) printf("    returning %d\n", rv);
	return rv;
}

int color_from_bits(unsigned int bits)
{
	int rval;

	if(DEBUG)printf("DEBUG: color form bits %X\n", bits);

	rval = extract(bits, COLOR_SHIFT, COLOR_MASK); 
	return rval;

}

