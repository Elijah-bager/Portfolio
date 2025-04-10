
// Copyright 2025 Neil Kirby not for distribution without written
// permission.

struct Stack {
	long *data;	// offset = 0
	long *tos;	// offset = 8 "tos" == top of stack
};

typedef long (*math_function)(long A, long B);

struct Op {
	char tag;	// offset = 0
	int type;	// offset = 4
	long value;	// offset = 8
	math_function f; // offset = 16
};

