
// Copyright 2025 Neil Kirby, not for disclosure wtihout written permission
//

#include <stdio.h>

#include "structs.h"

long add(long A, long B);
long divide(long A, long B);
long multiply(long A, long B);
long subtract(long A, long B);

long loop(struct Stack *stack, struct Op *actions[], long count);


#define OPERATOR 0
#define VALUE 1
//I want this in the heap to make it easy to spot by pointer values
static long values[20] = {0};
static struct Op
	bad= {'X', 1024*1024, 1024*1024, NULL},
	v1 = {'#', VALUE, 1, NULL},
	v2 = {'#', VALUE, 2, NULL},
	v3 = {'#', VALUE, 3, NULL},
	v4 = {'#', VALUE, 4, NULL},
	v5 = {'#', VALUE, 5, NULL},
	v6 = {'#', VALUE, 6, NULL},
	v7 = {'#', VALUE, 7, NULL},
	v8 = {'#', VALUE, 8, NULL},
	v9 = {'#', VALUE, 9, NULL},
	o_plus = {'+', OPERATOR, -1, add},
	o_minus = {'-', OPERATOR, -1, subtract},
	o_mult = {'*', OPERATOR, -1, multiply},
	o_div = {'/', OPERATOR, -1, divide};

int main()
{
	struct Stack actual = {values, values+19}, *stack = &actual;
	struct Op *actions[] = {
		&bad,
		&v3, &v2, &v4,
		&o_plus, 
		&o_mult, 
		&v9,
		&o_div,
		&v1,
		&o_minus
	};

	values[19] = -1; // a value we should never see in use
	long count = sizeof(actions) / sizeof(actions[0]);

	printf("Calling loop with %ld actions.\n", count);

	long final = loop(stack, actions, count);
	printf("Final result was %ld\n", final);
}


void print_top(struct Stack *stack)
{
	printf("%ld is at the top of the stack at %p\n", *stack->tos, 
		(void *) stack->tos);
}



