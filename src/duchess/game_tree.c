/**
 *	game_tree.c
 *	Author: Adam Pickersgill
 *
 *
 */
 #include "game_tree.h"

/*
struct TreeNode* newTreeNode2(struct Graph* state) {
    struct TreeNode* newNode = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    newNode->state = state;
    newNode->children = NULL;
}

struct TreeNode* newTreeNode3(struct Graph* state, int type, int moveVal, int lev) {
    struct TreeNode* newNode = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    newNode->state = state;
    newNode->children = createQueue();
    newNode->moveType = type;
    newNode->moveValue = moveVal;
    newNode->level = lev;
    newNode->evaluation = NULL;
}*/

struct TreeNode* newTreeNode(struct Graph* state) {
    struct TreeNode* newNode = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    //newNode->state = state;

    newNode->state = (struct Graph*) malloc(sizeof(struct Graph));
    newNode->state->player = (struct Player*) malloc(sizeof(struct Player));
    newNode->state->opponent = (struct Player*) malloc(sizeof(struct Player));
    newNode->state->array = state->array;
    newNode->state->player->position = state->player->position;
    newNode->state->player->num_walls = state->player->num_walls;
    newNode->state->player->label = state->player->label;
    newNode->state->opponent->position = state->opponent->position;
    newNode->state->opponent->num_walls = state->opponent->num_walls;
    newNode->state->opponent->label = state->opponent->label;
    newNode->child = NULL;
    newNode->sibling = NULL;
};


struct TreeNode* createGameTree(struct Graph* currentState, int depth, int turn) {
    struct TreeNode* newNode = newTreeNode(currentState);

    if (depth == 0) {
        return newNode;
    }

    struct TreeNode* parent = newNode;
    struct Player* playerTurn;
    int doneMoves = 0;
    int left = 0;
    int right = 0;
    int fwd = 0;
    int bwd = 0;
    int firstChild = 1;
    int j, nextTurn;
    if (turn == 1) {
        playerTurn = currentState->player;
        nextTurn = 0;
    } else {
        playerTurn = currentState->opponent;
        nextTurn = 1;
    }
    struct Graph* nextState = (struct Graph*)malloc(sizeof(struct Graph));
    nextState->player = (struct Player*)malloc(sizeof(struct Player));
    nextState->opponent = (struct Player*)malloc(sizeof(struct Player));

    nextState->array = currentState->array;

    nextState->player->position = currentState->player->position;
    nextState->player->num_walls = currentState->player->num_walls;
    nextState->player->label = currentState->player->label;

    nextState->opponent->position = currentState->opponent->position;
    nextState->opponent->num_walls = currentState->opponent->num_walls;
    nextState->opponent->label = currentState->opponent->label;

    while (!doneMoves) {
        if (firstChild == 1) {
            //First Child
            //Forward move
            if (isValidMove(playerTurn->position, playerTurn->position + dim)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position + dim;
                } else {
                    nextState->opponent->position = playerTurn->position + dim;
                }
                parent->child = createGameTree(nextState, depth - 1, nextTurn);
                fwd = 1;
            //Backward move
            } else if (isValidMove(playerTurn->position, playerTurn->position - dim)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position - dim;
                } else {
                    nextState->opponent->position = playerTurn->position - dim;
                }
                parent->child = createGameTree(nextState, depth - 1, nextTurn);
                bwd = 1;
            //Move Right
            } else if (isValidMove(playerTurn->position, playerTurn->position + 1)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position + 1;
                } else {
                    nextState->opponent->position = playerTurn->position + 1;
                }
                parent->child = createGameTree(nextState, depth - 1, nextTurn);
                right = 1;
            //Move Left
            } else if (isValidMove(playerTurn->position, playerTurn->position - 1)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position - 1;
                } else {
                    nextState->opponent->position = playerTurn->position - 1;
                }
                parent->child = createGameTree(nextState, depth - 1, nextTurn);
                left = 1;
            }
            parent = parent->child;
            firstChild = 0;
        } else {
            if (fwd == 0 && isValidMove(playerTurn->position, playerTurn->position + dim)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position + dim;
                } else {
                    nextState->opponent->position = playerTurn->position + dim;
                }
                parent->sibling = createGameTree(nextState, depth - 1, nextTurn);
                parent = parent->sibling;
                fwd = 1;
            }
            if (bwd == 0 && isValidMove(playerTurn->position, playerTurn->position - dim)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position - dim;
                } else {
                    nextState->opponent->position = playerTurn->position - dim;
                }
                parent->sibling = createGameTree(nextState, depth - 1, nextTurn);
                parent = parent->sibling;
                bwd = 1;
            }
            if (right == 0 && isValidMove(playerTurn->position, playerTurn->position + 1)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position + 1;
                } else {
                    nextState->opponent->position = playerTurn->position + 1;
                }
                parent->sibling = createGameTree(nextState, depth - 1, nextTurn);
                parent = parent->sibling;
                right = 1;
            }
            if (left == 0 && isValidMove(playerTurn->position, playerTurn->position - 1)) {
                if (turn == 1) {
                    nextState->player->position = playerTurn->position - 1;
                } else {
                    nextState->opponent->position = playerTurn->position - 1;
                }
                parent->sibling = createGameTree(nextState, depth - 1, nextTurn);
                parent = parent->sibling;
            }
            doneMoves = 1;
        }
    }
   // printf("NEWNODE: %d\n", newNode->state->player->position);
    return newNode;
}
