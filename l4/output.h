/* Elijah Baugher */

void bad_bits_message( char *where, unsigned int bits);
void bad_missile_message(struct Missile *mis);
void dead_base_message(struct Aimpoint *base);
void dead_missile_message(struct Missile *mis);
void detonation_message(int color, double X, double Y);
void exit_area_message(int color, double X, double Y);
void final_output(struct Sim *sim);
void fire_message(struct Aimpoint *base);
void master_output(struct Sim *sim);
void scanf_message(char *who, int got, int wanted);
