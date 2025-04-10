/* Elijah Baugher */

void aimpoint_motion(struct Aimpoint *aim, struct Sim *sim);
bool have_bases(struct Sim *sim);
bool in_beam(void *data, void *helper);
bool leftmost_missile(void *data1, void *data2);
bool lowest_missile(void *data1, void *data2);
bool rightmost_missile(void *data1, void *data2);
void update_aimpoints(struct Sim *sim);
void update_an_aimpoint(struct Sim *sim, struct Aimpoint *aim);
