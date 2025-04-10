
//Copyright 2025 Neil Kirby, not for disclosure without permission
//

#include <stdio.h>

#include "structs.h"

long pop_value( struct Stack *stack);
void push_value(struct Stack *stack, long value);

void e_shim( struct Stack *stack, struct Op *op);

void print_top(struct Stack *stack);


// MAKE THIS THE BONUS

long loop(struct Stack *stack, struct Op *actions[], long count)
{
	printf("%ld actions in the array.\n", count);
	print_top(stack);

	for(long i = 0; i<count; i++)
	{
	    e_shim(stack, actions[i]);
	    print_top(stack);
	}
	long rval = pop_value(stack);
	return rval;
}

