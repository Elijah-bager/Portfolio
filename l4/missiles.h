/* Elijah Baugher */

void add_missile(struct Sim *sim, struct Missile *mis);
bool by_altitude(void *data1, void *data2);
void destroy_missile(void *data);
bool missile_is_inflight(struct Missile *mis);
bool missile_not_inflight(void *data);
void purge_missiles(struct Sim *sim);
bool too_close(struct Missile *mis, struct Aimpoint *base);
void update_a_missile(void *data);
void update_missiles(struct Sim *sim);
void vaporize_bases(struct Missile *mis);
