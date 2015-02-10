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
    return node->state->player->position*node->state->opponent->position;
}

int max(int a, int b) {
    return (a > b ? a : b);
}

int min(int a, int b) {
    return (a < b ? a : b);
}


int minimax(struct TreeNode* node, int depth, int player) {
    int value;
    struct TreeNode* children;

    if (depth == 0 || !node->child) {
        return getEvaluation(node);
    }

    if (player == MAX_PLAYER) {
        int bestValue = INT_MIN;
        children = node->child;
        while (children) {
            value = minimax(children, depth - 1, MIN_PLAYER);
            bestValue = max(bestValue, value);
            children = children->sibling;
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        children = node->child;
        while (children) {
            value = minimax(children, depth - 1, MAX_PLAYER);
            bestValue = min(bestValue, value);
            children = children->sibling;
        }
        return bestValue;
    }
}
