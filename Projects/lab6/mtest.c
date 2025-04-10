
/* Copyright 2025 Neil Kirby, not for disclosure without written permission */
#include <stdio.h>

#include "structs.h"

long add(long A, long B);
long divide(long A, long B);
long multiply(long A, long B);
long subtract(long A, long B);



void mtest(long A, long B, char tag, math_function f)
{
	long C;
	printf("mtest: %ld %c %ld = \n", A, tag, B);
	fflush(stdout);
	C = f(A,B);
	printf("    %ld\n\n", C);
}

int main()
{
	// uncomment these as you implement them
	mtest(1, 3, '+', add);
	mtest(100, 300, '-', subtract);
	mtest(-10, 20, '*', multiply);
	mtest(64, -8, '/', divide);
}

