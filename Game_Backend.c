#include "Game_Backend.h"
#include <unistd.h>
#include <sys/select.h>

const char *game_resource_to_str(int resource){
	switch(resource){
		case ORE: return "ORE"; break;
		case WHEAT: return "WHEAT"; break;
		case SHEEP: return "SHEEP"; break;
	   	case WOOD: return "WOOD"; break;
	   	case BRICK: return "BRICK"; break;
	   	case GOLD: return "GOLD"; break;
	}
	return "WHAT";
}

char *game_serialize_player_board(PlayerBoard *pb){
	char *str = calloc(2048, sizeof(char));
	int pos = 0, chars;
	//Roads
	for(int i = 0; i < pb->roads->size; i++){
		chars = sprintf(str+pos, "%d", get_int(pb->roads, i));
		if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		if(i != pb->roads->size - 1){
			chars = sprintf(str+pos, ",");
			if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		}
	}		
	chars = sprintf(str+pos, ":");
	if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
	//Settles
	for(int i = 0; i < pb->settlements->size; i++){
		chars = sprintf(str+pos, "%d", get_int(pb->settlements, i));
		if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		if(i != pb->settlements->size - 1){
			chars = sprintf(str+pos, ",");
			if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		}
	}		
	chars = sprintf(str+pos, ":");
	if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
	//Cities
	for(int i = 0; i < pb->cities->size; i++){
		chars = sprintf(str+pos, "%d", get_int(pb->cities, i));
		if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		if(i != pb->cities->size - 1){
			chars = sprintf(str+pos, ",");
			if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		}
	}		
	chars = sprintf(str+pos, ";");
	if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
	//Resources
	for(int i = 0; i < pb->resources.positions->size; i++){
		Vec2d position = get_Vec2d(pb->resources.positions, i);
		chars = sprintf(str+pos, "(%d,%d)", position.x, position.y);
		if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		chars = sprintf(str+pos, "%d/", get_int(pb->resources.dice, i));
		if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		chars = sprintf(str+pos, "%s", game_resource_to_str(get_int(pb->resources.resources, i)));
		if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		if(i != pb->resources.positions->size - 1){
			chars = sprintf(str+pos, ",");
			if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
		}
	}
	chars = sprintf(str+pos, ";");
	if(chars > 0) { pos += chars; } else { fprintf(stderr, "Error in string serialization"); exit(EXIT_FAILURE); }
	return str;
}


void game_backend_init_game(int player1_fd, int player2_fd, PlayerBoard *player1_board, PlayerBoard *player2_board){
	(void)player1_fd;
	(void)player2_fd;
	//Roads
	player1_board->roads = createDynamicArray_int();
	append_int(player1_board->roads, 0);
	player2_board->roads = createDynamicArray_int();
	append_int(player2_board->roads, 0);
	//Settlements
	player1_board->settlements = createDynamicArray_int();
	append_int(player1_board->settlements, -1);
	append_int(player1_board->settlements, 1);
	player2_board->settlements = createDynamicArray_int();
	append_int(player2_board->settlements, -1);
	append_int(player2_board->settlements, 1);
	//Cities
	player1_board->cities = createDynamicArray_int();
	player2_board->cities = createDynamicArray_int();
	//Resources. Player 1 is blue, player 2 is red
	//@TODO: implement player chooses resources positions
	player1_board->resources.positions = createDynamicArray_Vec2d();
	player1_board->resources.resources = createDynamicArray_int();
	player1_board->resources.dice = createDynamicArray_int();
	append_Vec2d(player1_board->resources.positions, (Vec2d){.x=0,  .y=-1});
	append_Vec2d(player1_board->resources.positions, (Vec2d){.x=-2, .y=-1});
	append_Vec2d(player1_board->resources.positions, (Vec2d){.x=-2, .y= 1});
	append_Vec2d(player1_board->resources.positions, (Vec2d){.x=0,  .y= 1});
	append_Vec2d(player1_board->resources.positions, (Vec2d){.x=2,  .y= 1});
	append_Vec2d(player1_board->resources.positions, (Vec2d){.x=2,  .y=-1});

	append_int(player1_board->resources.resources, SHEEP);
   	append_int(player1_board->resources.resources, BRICK);
	append_int(player1_board->resources.resources, WOOD);
	append_int(player1_board->resources.resources, GOLD);
	append_int(player1_board->resources.resources, WHEAT);
	append_int(player1_board->resources.resources, ORE);

	append_int(player1_board->resources.dice, 1);
   	append_int(player1_board->resources.dice, 2);
	append_int(player1_board->resources.dice, 3);
	append_int(player1_board->resources.dice, 4);
	append_int(player1_board->resources.dice, 5);
	append_int(player1_board->resources.dice, 6);

	player2_board->resources.positions = createDynamicArray_Vec2d();
	player2_board->resources.resources = createDynamicArray_int();
	player2_board->resources.dice = createDynamicArray_int();
	append_Vec2d(player2_board->resources.positions, (Vec2d){.x=0,  .y=1});
	append_Vec2d(player2_board->resources.positions, (Vec2d){.x=-2, .y=1});
	append_Vec2d(player2_board->resources.positions, (Vec2d){.x=-2, .y=-1});
	append_Vec2d(player2_board->resources.positions, (Vec2d){.x=0,  .y=-1});
	append_Vec2d(player2_board->resources.positions, (Vec2d){.x=2,  .y=-1});
	append_Vec2d(player2_board->resources.positions, (Vec2d){.x=2,  .y=1});

	append_int(player2_board->resources.resources, GOLD);
	append_int(player2_board->resources.resources, WOOD);
   	append_int(player2_board->resources.resources, BRICK);
	append_int(player2_board->resources.resources, SHEEP);
	append_int(player2_board->resources.resources, ORE);
	append_int(player2_board->resources.resources, WHEAT);

	append_int(player2_board->resources.dice, 1);
   	append_int(player2_board->resources.dice, 2);
	append_int(player2_board->resources.dice, 3);
	append_int(player2_board->resources.dice, 4);
	append_int(player2_board->resources.dice, 5);
	append_int(player2_board->resources.dice, 6);

	char *player1_serialized = game_serialize_player_board(player1_board);
	char *player2_serialized = game_serialize_player_board(player2_board);
	//printf("\n\nPlayer 1:\n%s\n\nPlayer 2:\n%s\n", player1_serialized, player2_serialized);
	char init_game_message[2048] = {0};	
	sprintf(init_game_message, "You have been paired up! You are the Blue player.\nYour board is: %s\n", player1_serialized);
	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(player1_fd, &writefds);
	while(select(player1_fd + 1, NULL, &writefds, NULL, NULL) != 1);
	if(write(player1_fd, init_game_message, strlen(init_game_message) + 1) == -1){
		fprintf(stderr, "Error sending message to client.\n\n");
		exit(EXIT_FAILURE);
	}

	sprintf(init_game_message, "You have been paired up! You are the Red player.\nYour board is: %s\n", player2_serialized);
	FD_ZERO(&writefds);
	FD_SET(player2_fd, &writefds);
	while(select(player2_fd + 1, NULL, &writefds, NULL, NULL) != 1);
	if(write(player2_fd, init_game_message, strlen(init_game_message) + 1) == -1){
		fprintf(stderr, "Error sending message to client.\n\n");
		exit(EXIT_FAILURE);
	}
}
