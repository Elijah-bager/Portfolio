/* Copyright 2024 Neil Kirby, all rights reserved. */
/* Do not publish this code without written permission */
#include <stdbool.h>
/* Neil Kirby */

bool mc_base(int color, int score, double ground_X);
int mc_beam(int color, double X, double width);
void mc_beep();
bool mc_boom (int color, double X, double Y);
void mc_clear();
void mc_flash();
int mc_getch();
int mc_initialize();
bool mc_missile(int color, double X, double Y);
void mc_refresh();
void mc_sim_time(int milliseconds);
void mc_status(const char *statstr);
void mc_teardown();
