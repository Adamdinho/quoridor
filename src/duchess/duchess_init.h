/*************************************************************
* File Name : duchess_init.h
* ECSE 456 Design Project
* Creation Date : 10/28/2014
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: header file for the initialization routine,
* designed modularly to be accessed by other modules.
**************************************************************/

#ifndef DUCHINIT_H
#define DUCHINIT_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "game_tree.h"
#include "game_state.h"
#include "common_structs.h"
#include "choose_move_or_wall.h"

#define SIZEOFINT sizeof(int)
#define P_SIZE sizeof(struct Player)
#define G_SIZE sizeof(struct Graph)


//extern struct Player* maxPlayer;
//extern struct Player* minPlayer;
//
extern struct Graph* graph;

int init(int p_1or2,int* eval_weights);
void start_turn(char* last_turn);

#endif
