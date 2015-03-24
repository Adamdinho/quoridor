/*************************************************************
* \file choose_move_or_wall.c
* ECSE 456 Design Project
* Creation Date : 10/28/2014
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: Contains the logic to begin evaluating the next
* possible moves beginning with the choice to play a wall or
* move the pawn of the player.
**************************************************************/

#include "choose_move_or_wall.h"

double numbers[10] = {2.564,8.546,25.548,46.54897,24.56576,22.25616,46.8876,100.528,884.248,94.58};


int max(int a, int b) {
    return (a > b ? a : b);
}

int min(int a, int b) {
    return (a < b ? a : b);
}

int getEvaluation(struct TreeNode* node) {
	int i,j,x;
	for (x = 0; x < NUM; x++) {
	for (i = 0; i < 10; i++) {
		for (j = 0; j <10; j++) {
			numbers[i] = numbers[i]*numbers[j] - numbers[j]/numbers[i];
		}
	}
	}
    //printf("Evaluated Value: %d\n", evaluation);
    //printf("maxPlayerWalls: %d, minPlayerWalls: %d\n", maxPlayer->num_walls, minPlayer->num_walls);
    return node->maxPos * node->minPos; //node->player->position * node->opponent->position;
}

int minimax(struct TreeNode* node, int depth, int player) {
    int value;
    struct TreeNode* children;
    int wallPlaced = 0;
    int valid = 1;

    if (depth == 0 || !node->child) {
        return getEvaluation(node);
    }

    if (player == MAX_PLAYER) {
        int bestValue = INT_MIN;
        children = node->child;
        while (children) {
            struct Player* playerTurn = children->turn == MAX_PLAYER ? minPlayer : maxPlayer;
            if (children->wall[0] != -1) {
                if (placeWall(graph, children->wall[0], children->wall[1], children->wall[2], children->maxPos, children->minPos)) {
                    playerTurn->num_walls--;
                    wallPlaced = 1;
                    valid = 1;
                } else {
                    valid = 0;
                    wallPlaced = 0;
                }
            }
            if (valid) {
                value = minimax(children, depth - 1, MIN_PLAYER);
                bestValue = max(bestValue, value);
                if (depth == DEPTH && bestValue == value) {
                    nextState = children;
                }
            }
            if (wallPlaced) {
                removeWall(graph, children->wall[0], children->wall[1], children->wall[2]);
                playerTurn->num_walls++;
            }
            children = children->sibling;
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        children = node->child;
        while (children) {
            struct Player* playerTurn = children->turn == MAX_PLAYER ? minPlayer : maxPlayer;
            if (children->wall[0] != -1) {
                if (placeWall(graph, children->wall[0], children->wall[1], children->wall[2], children->maxPos, children->minPos)) {
                    playerTurn->num_walls--;
                    wallPlaced = 1;
                    valid = 1;
                } else {
                    valid = 0;
                    wallPlaced = 0;
                }
            }
            if (valid) {
                value = minimax(children, depth - 1, MAX_PLAYER);
                bestValue = min(bestValue, value);
                if (depth == DEPTH && bestValue == value) {
                    nextState = children;
                }
            }
            if (wallPlaced) {
                removeWall(graph, children->wall[0], children->wall[1], children->wall[2]);
                playerTurn->num_walls++;
            }
            children = children->sibling;
        }
        return bestValue;
    }
}
