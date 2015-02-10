
#ifndef GSTATE_H
#define GSTATE_H

#include "duchess_init.h"

#define MAX_PLAYER 1
#define MIN_PLAYER 0
#define NUM_NODES 81
#define DEPTH 6
#define NUM 20
#define dim 9

#define WALL_UP 0
#define WALL_DOWN 1
#define WALL_LEFT 2
#define WALL_RIGHT 3

#undef max
#undef min

int isValidMove(int currentPosition, int targetPosition);
int isValidWall(int square1, int square2);
int placeWall(struct Graph* graph, int square1, int square2, int placement);
void placeWallPiece(struct Graph* graph, int square1, int square2);

#endif
