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
}*

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

    return newNode;
}; */

// TODO (Omar#1#): Add code for the new TreeNode Struct
struct TreeNode* newTreeNode(int maxPosition, int minPosition, int wall[3], int turn) {
    struct TreeNode* newNode = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    if (newNode == NULL) {
        printf("\nMalloc failed Line 71\n");
        exit(0);
    }

    newNode->maxPos = maxPosition;
    newNode->minPos = minPosition;
    newNode->wall[0] = wall[0];
    newNode->wall[1] = wall[1];
    newNode->wall[2] = wall[2];
    newNode->turn = turn;

    newNode->child = NULL;
    newNode->sibling = NULL;

    return newNode;
};


struct TreeNode* createGameTree(int wall[3], int depth, int turn, int currentMaxPos, int currentMinPos) {
    struct TreeNode* newNode = newTreeNode(currentMaxPos, currentMinPos, wall, turn);

    if (depth == 0) {
        return newNode;
    }

    int nextMaxPos, nextMinPos;
    int neighbor[4] = {dim, -dim, 1, -1};
    int wallOrientation[4] = {WALL_LEFT, WALL_UP, WALL_RIGHT, WALL_DOWN};
    //int nextMove = 0; //No movement
    int nextWall[3] = {-1 , -1, -1}; //No wall
    struct TreeNode* parent = newNode;
    int maxPosition = currentMaxPos;
    int minPosition = currentMinPos;
    int playerPosition, opponentPosition;
    //struct Player* playerTurn;
    //struct Player* playerOpponent;
    int doneMoves = 0;
    int left = 0;
    int right = 0;
    int fwd = 0;
    int bwd = 0;
    int firstChild = 1;
    int nextTurn;
    if (turn == 1) {
        playerPosition = maxPosition;
        opponentPosition = minPosition;
        //playerTurn = maxPlayer; //player;
        //playerOpponent = minPlayer; //Opponent
        nextTurn = 0;
    } else {
        playerPosition = minPosition;
        opponentPosition = maxPosition;
        //playerTurn = minPlayer; //opponent;
        //playerOpponent = maxPlayer;
        nextTurn = 1;
    }

    while (!doneMoves) {
        if (firstChild == 1) {
            //First Child
            //Forward move
            if (isValidMove(playerPosition, playerPosition + dim)) {
                //nextMove = dim;
                nextMaxPos = turn == 1 ? maxPosition + dim : maxPosition;
                nextMinPos = turn == 0 ? minPosition + dim : minPosition;
                //parent->move = dim;
                //parent->turn = turn;

                parent->child = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                fwd = 1;
            //Backward move
            } else if (isValidMove(playerPosition, playerPosition - dim)) {
                //nextMove = -dim;
                nextMaxPos = turn == 1 ? maxPosition - dim : maxPosition;
                nextMinPos = turn == 0 ? minPosition - dim : minPosition;

                parent->child = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                bwd = 1;
            //Move Right
            } else if (isValidMove(playerPosition, playerPosition + 1)) {
                //nextMove = 1;
                nextMaxPos = turn == 1 ? maxPosition + 1 : maxPosition;
                nextMinPos = turn == 0 ? minPosition + 1 : minPosition;

                parent->child = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                right = 1;
            //Move Left
            } else if (isValidMove(playerPosition, playerPosition - 1)) {
                //nextMove = -1;
                nextMaxPos = turn == 1 ? maxPosition - 1 : maxPosition;
                nextMinPos = turn == 0 ? minPosition - 1 : minPosition;

                parent->child = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                left = 1;
            }
            parent = parent->child;
            firstChild = 0;
        } else {
            if (fwd == 0 && isValidMove(playerPosition, playerPosition + dim)) {
                //nextMove = dim;
                nextMaxPos = turn == 1 ? maxPosition + dim : maxPosition;
                nextMinPos = turn == 0 ? minPosition + dim : minPosition;

                parent->sibling = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                parent = parent->sibling;
                fwd = 1;
            }
            if (bwd == 0 && isValidMove(playerPosition, playerPosition - dim)) {
                //nextMove = -dim;
                nextMaxPos = turn == 1 ? maxPosition - dim : maxPosition;
                nextMinPos = turn == 0 ? minPosition - dim : minPosition;

                parent->sibling = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                parent = parent->sibling;
                bwd = 1;
            }
            if (right == 0 && isValidMove(playerPosition, playerPosition + 1)) {
                //nextMove = 1;
                nextMaxPos = turn == 1 ? maxPosition + 1 : maxPosition;
                nextMinPos = turn == 0 ? minPosition + 1 : minPosition;

                parent->sibling = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                parent = parent->sibling;
                right = 1;
            }
            if (left == 0 && isValidMove(playerPosition, playerPosition - 1)) {
                //nextMove = -1;
                nextMaxPos = turn == 1 ? maxPosition - 1 : maxPosition;
                nextMinPos = turn == 0 ? minPosition - 1 : minPosition;

                parent->sibling = createGameTree(nextWall, depth - 1, nextTurn, nextMaxPos, nextMinPos);
                parent = parent->sibling;
            }
            //------WALLS------
			//Place Walls around player (Defensive)
			int i, j;
			for (i = 0, j = 0; i < 8; i++, j++) {
                nextWall[0] = playerPosition;
                nextWall[1] = playerPosition + neighbor[j%4];
                nextWall[2] = wallOrientation[i/2];
                parent->sibling = createGameTree(nextWall, depth - 1, nextTurn, maxPosition, minPosition);
                parent = parent->sibling;
			}
            //Place Walls around opponent (Offensive)
			for (i = 0, j = 0; i < 8; i++, j++) {
                nextWall[0] = opponentPosition;
                nextWall[1] = opponentPosition + neighbor[j%4];
                nextWall[2] = wallOrientation[i/2];
                parent->sibling = createGameTree(nextWall, depth - 1, nextTurn, maxPosition, minPosition);
                parent = parent->sibling;
			}
			//Place Walls Around Opponent

            doneMoves = 1;
        }
    }
    return newNode;
}


// A utility function to print the adjacency list representation of graph
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < NUM_NODES + 2; v++)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

void printRecursiveTree(struct TreeNode* tree, int level) {
    if (!tree) {
        return;
    }
    struct Player* playerTurn = tree->turn == MAX_PLAYER ? minPlayer : maxPlayer;
    int wallPlaced = 0;
    int valid = 1;
    if (tree->wall[0] != -1) {
        //If wall is valid
        //printf("Attempting to place wall: %d, %d, %d\n", tree->wall[0], tree->wall[1], tree->wall[2]);
        if (placeWall(graph, tree->wall[0], tree->wall[1], tree->wall[2], tree->maxPos, tree->minPos)) {
            //printf("Placing Wall: %d, %d, %d\n", tree->wall[0], tree->wall[1], tree->wall[2]);
            playerTurn->num_walls--;
            wallPlaced = 1;
        //Wall is invalid --> subtree is invalid. Delete subtree?
        } else {
            //printf("INVALID SUBTREE\n");
            valid = 0;
            wallPlaced = 0;
        }
    }

    if (valid) {
        printf("%d:\t\tPlayer: %d\t%d\tOpponent: %d\t%d\n\n", level, tree->maxPos, maxPlayer->num_walls, tree->minPos, minPlayer->num_walls);
        if (wallPlaced) {
           removeWall(graph, tree->wall[0], tree->wall[1], tree->wall[2]);
           playerTurn->num_walls++;
        }

    }

    //maxPlayer->position = tree->turn == MIN_PLAYER ? parentMaxPos - tree->move : maxPlayer->position;
    //minPlayer->position = tree->turn == MAX_PLAYER ? parentMinPos - tree->move : minPlayer->position;


    printRecursiveTree(tree->sibling, level);

    //maxPlayer->position = tree->turn == MIN_PLAYER ? parentMaxPosition + tree->move : maxPlayer->position;
    //minPlayer->position = tree->turn == MAX_PLAYER ? parentMinPosition + tree->move : minPlayer->position;
    if (valid) {
        if (wallPlaced) {
            placeWall(graph, tree->wall[0], tree->wall[1], tree->wall[2], tree->maxPos, tree->minPos);
            //playerTurn->num_walls--;
        }
        printRecursiveTree(tree->child, level + 1);
    }
}

void printRecursiveTree2(struct TreeNode* tree, int level) {
    if (level == 0) {
        printf("%d:\t\tPlayer: %d\t%d\tOpponent: %d\t%d\n\n", level, tree->maxPos, maxPlayer->num_walls, tree->minPos, minPlayer->num_walls);
        printRecursiveTree2(tree, level + 1);
        return;
    }
    struct TreeNode* children = tree->child;
    int wallPlaced = 0;
    int valid = 1;
    while(children) {
        struct Player* playerTurn = children->turn == MAX_PLAYER ? minPlayer : maxPlayer;
        if (children->wall[0] != -1) {
            if (placeWall(graph, children->wall[0], children->wall[1], children->wall[2], children->maxPos, children->minPos)) {
                //printf("Placing Wall: %d, %d, %d\n", children->wall[0], children->wall[1], children->wall[2]);
                playerTurn->num_walls--;
                wallPlaced = 1;
                valid = 1;
            } else {
                //printf("INVALID WALL\n");
                valid = 0;
                wallPlaced = 0;
            }
        }
        if (valid == 1) {
            printf("%d:\t\tPlayer: %d\t%d\tOpponent: %d\t%d\n\n", level, children->maxPos, maxPlayer->num_walls, children->minPos, minPlayer->num_walls);
            printRecursiveTree2(children, level + 1);
        }
        if (wallPlaced) {
            removeWall(graph, children->wall[0], children->wall[1], children->wall[2]);
            playerTurn->num_walls++;
        }
        children = children->sibling;
    }
}
