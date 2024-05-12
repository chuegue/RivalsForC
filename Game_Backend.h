#ifndef __GAME_BACKEND__
#define __GAME_BACKEND__
#include "DA.h"
#include "Math.h"

//DECLARE_DYNAMIC_ARRAY(int)
DECLARE_DYNAMIC_ARRAY(Vec2d)

enum GeneralBuildings {
	ROAD = 0,
	SETTLE,
	CITY,
};

enum Resources {
	ORE = 0,
	WHEAT,
	SHEEP,
	WOOD,
	BRICK,
	GOLD,
};

typedef struct {
	DynamicArray_int *roads;
	DynamicArray_int *settlements;
	DynamicArray_int *cities;
	struct {
		DynamicArray_Vec2d *positions;
		DynamicArray_int *resources;
		DynamicArray_int *dice;
	} resources;
} PlayerBoard;


void game_backend_init_game(int player1_fd, int player2_fd, PlayerBoard *player1_board, PlayerBoard *player2_board);

#endif // __GAME_BACKEND__
