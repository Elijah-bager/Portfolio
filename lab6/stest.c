
/* Copyright 2025 Neil Kirby, not for disclosure without written permission */
#include <stdio.h>

#include "structs.h"

long pop_value( struct Stack *stack);
void push_value(struct Stack *stack, long value);

// this is prototype quality code with magic numbers and file scope
// variables.

static long values[20];

void dump(struct Stack *stack)
{
	printf("Dumping the stack:\n");
	if(stack->data != values)
	    printf("STACK IS CORRUPT - data pointer trashed\n");
	if(stack->tos < values)
	    printf("STACK IS CORRUPT - tos too low\n");
	if(stack->tos > values+19)
	    printf("STACK IS CORRUPT - tos too high\n");

	for(long i = 19; i>=0; i--)
	{
	    long *ptr = stack->data+i;
	    printf("%p    %ld", (void *)ptr, *ptr);
	    if(ptr == stack->tos)printf("	<- TOS");
	    printf("\n");
	}
	printf("\n");
}


int main()
{
	struct Stack actual = {values, values+19}, *stack = &actual;
	long val;

	values[19] = -1; // a value we should never see in use

	dump(stack);
	val = 20; printf("pushing %ld\n", val);
	push_value(stack, val);
	dump(stack);

	val = 30; printf("pushing %ld\n", val);
	push_value(stack, val);
	dump(stack);

	val = 40; printf("pushing %ld\n", val);
	push_value(stack, val);
	dump(stack);

	printf("popping...\n");
	val = pop_value(stack);
	printf("%ld\n", val);
	dump(stack);

	val = 500; printf("pushing %ld\n", val);
	push_value(stack, val);
	dump(stack);

	printf("popping...\n");
	val = pop_value(stack);
	printf("%ld\n", val);
	dump(stack);

	printf("popping...\n");
	val = pop_value(stack);
	printf("%ld\n", val);
	dump(stack);

	return 0;

}



