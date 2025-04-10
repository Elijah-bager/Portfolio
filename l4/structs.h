// COpyright 2025 Neil Kirby, not for disclosure without permission
// structs.h

struct Aimpoint {
	int color, score;
	double beam_width, ground_speed;
	double ground_X;
};

#define BASE_COUNT 8
struct Sim {
	double elapsed;
	void *inflight;	// mssiles and ABM in the game
	struct Aimpoint bases[BASE_COUNT];  // All you base are belong to us
};

struct Missile {
	int color;
	double x_position, y_position;
	double x_velocity, y_velocity;
	double det_alt;
	struct Sim *sim;
};

	

