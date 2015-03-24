#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>

#define MAX_PLAYER 1
#define MIN_PLAYER 0
#define NUM_NODES 25
#define DEPTH 4
#define NUM 20

#define WALL_RADIUS 1
#define WALL_UP 0
#define WALL_DOWN 1
#define WALL_LEFT 2
#define WALL_RIGHT 3

#undef max
#undef min

static int const dim = sqrt(NUM_NODES);
struct Player* maxPlayer;
struct Player* minPlayer;
struct TreeNode* nextState;
struct Graph* graph;
double numbers[10] = {2.564,8.546,25.548,46.54897,24.56576,22.25616,46.8876,100.528,884.248,94.58};

struct Player {
    int position;
    int num_walls;
    int label;
};

struct TreeNode {
    int wall[3];
    int maxPos;
    int minPos;
    int turn;
    struct TreeNode* child;
    struct TreeNode* sibling;
};

struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode* head;
};

struct Graph {
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    if (newNode == NULL) {
        printf("\nMalloc failed Line 60\n");
        exit(0);
    }
    newNode->dest = dest;
    newNode->next = NULL;
};

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

struct Graph* createBoard() {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    if (graph == NULL) {
        printf("\nMalloc failed Line 105\n");
        exit(0);
    }

    graph->array = (struct AdjList*) malloc((NUM_NODES + 2) * sizeof(struct AdjList));
    if (graph->array == NULL) {
        printf("\nMalloc failed Line 111\n");
        exit(0);
    }

    int i, j;
    for (i = 0; i < NUM_NODES + 2; ++i) {
        graph->array[i].head = NULL;
    }

    //int dim = sqrt(NUM_NODES);
    for (i = 0; i < NUM_NODES + 2; ++i) {
        for (j = 0; j < NUM_NODES + 2; ++j) {
            //For one of the extra source nodes
            if (i == 0) {
                if (j > 0 && j <= dim) {
                    struct AdjListNode* newNode = newAdjListNode(j);
                    newNode->next = graph->array[i].head;
                    graph->array[i].head = newNode;
                }
            //For the other extra source node
            } else if (i == NUM_NODES + 1) {
                if (j < NUM_NODES + 1 && j > NUM_NODES - dim) {
                    struct AdjListNode* newNode = newAdjListNode(j);
                    newNode->next = graph->array[i].head;
                    graph->array[i].head = newNode;
                }
            //For all other nodes
            } else {
                //For the first row, add edges to source node
                if (i <= dim && j == 0) {
                    struct AdjListNode* newNode = newAdjListNode(j);
                    newNode->next = graph->array[i].head;
                    graph->array[i].head = newNode;
                //For the last row, add edges to source node
                } else if (i > NUM_NODES - dim && j == NUM_NODES + 1) {
                    struct AdjListNode* newNode = newAdjListNode(j);
                    newNode->next = graph->array[i].head;
                    graph->array[i].head = newNode;
                }
                if ((j != 0 && j!= NUM_NODES + 1) &&
                    ((j == i-1 && (i-1) % dim != 0) ||
                    (j == i+1 && i % dim != 0) ||
                    j == i + dim ||
                    j == i - dim)) {
                    struct AdjListNode* newNode = newAdjListNode(j);
                    newNode->next = graph->array[i].head;
                    graph->array[i].head = newNode;
                }
            }
        }
    }
    return graph;
}

int isValidMove(int currentPosition, int targetPosition) {
    //Trying to move to one of the extra source nodes
    if (targetPosition == 0 || targetPosition == (NUM_NODES + 1)) {
        return 0;
    }
    struct AdjListNode* it = graph->array[currentPosition].head;
    while (it) {
        //If an edge exists from currentPos to targetPos, valid move
        if (targetPosition == it->dest) {
            return 1;
        }
        //Check all edges
        it = it->next;
    }
    //No path to targetPosition possible. Invalid Move
    return 0;
}

//Does not yet take into account that there always needs to be a path to goal
//Also, does not handle intersecting walls
int isValidWall(int square1, int square2) {
    if (square1 <= 0 || square2 <= 0
        || square1 > NUM_NODES || square2 > NUM_NODES) {
        return 0;
    }
    struct AdjListNode* it = graph->array[square1].head;
    while (it) {
        //If an edge exists from square1 to square2, it can be blocked
        if (square2 == it->dest) {
            return 1;
        }
        //Check all edges
        it = it->next;
    }
    //No path to square2. Cannot place wall
    return 0;
}

//Checks is wall placement is valid. If so wall is placed, else returns.
//Code can be cleaner. Will modify later
int placeWall(struct Graph* graph, int square1, int square2, int placement, int playerPos, int opponentPos) {
    //int dim = sqrt(NUM_NODES);
    //Where the squares are relative to each other
    int difference = square1 - square2;
    //Horizontal Wall (placement can only be UP OR DOWN of the 2 squares)
    if (difference == -1 || difference == 1) {
        if (placement == WALL_UP) {
            if (isValidWall(square1, square1 - dim) &&
                isValidWall(square2, square2 - dim)) {
                    placeWallPiece(graph, square1, square1 - dim);
                    placeWallPiece(graph, square2, square2 - dim);
                    return 1;
                    /*printf("DISTANCES\n");
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER));
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER));
                    if (distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER) != 1000 &&
                        distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER) != 1000) {
                        return 1;
                    } else {
                        printf("NO PATH TO GOAL\n");
                        removeWall(graph, square1, square2, placement);
                    }*/

            }
        } else if (placement == WALL_DOWN) {
            if (isValidWall(square1, square1 + dim) &&
                isValidWall(square2, square2 + dim)) {
                    placeWallPiece(graph, square1, square1 + dim);
                    placeWallPiece(graph, square2, square2 + dim);
                    return 1;
                    /*
                    printf("DISTANCES\n");
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER));
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER));
                    if (distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER) != 1000 &&
                        distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER) != 1000) {
                        return 1;
                    } else {
                        printf("NO PATH TO GOAL\n");
                        removeWall(graph, square1, square2, placement);
                    } */
            }
        }
    //Vertical Wall (placement can only be LEFT or RIGHT of the 2 squares)
    } else if (difference == dim || difference == -dim) {
        if (placement == WALL_LEFT) {
            if (isValidWall(square1, square1 - 1) &&
                isValidWall(square2, square2 - 1)) {
                    placeWallPiece(graph, square1, square1 - 1);
                    placeWallPiece(graph, square2, square2 - 1);
                    return 1;
                    /*printf("DISTANCES\n");
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER));
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER));
                    if (distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER) != 1000 &&
                        distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER) != 1000) {
                        return 1;
                    } else {
                        printf("NO PATH TO GOAL\n");
                        removeWall(graph, square1, square2, placement);
                    } */
            }
        } else if (placement == WALL_RIGHT) {
            if (isValidWall(square1, square1 + 1) &&
                isValidWall(square2, square2 + 1)) {
                    placeWallPiece(graph, square1, square1 + 1);
                    placeWallPiece(graph, square2, square2 + 1);
                    return 1;
                    /*printf("DISTANCES\n");
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER));
                    printf("\n%d, %d\n", distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER));
                    if (distanceToGoal(graph, playerPos, opponentPos, MAX_PLAYER) != 1000 &&
                        distanceToGoal(graph, playerPos, opponentPos, MIN_PLAYER) != 1000) {
                        return 1;
                    } else {
                        printf("NO PATH TO GOAL\n");
                        removeWall(graph, square1, square2, placement);
                    } */
            }
        }
    }
    return 0;
}

//Actually places the wall by removing edges from the graph
void placeWallPiece(struct Graph* graph, int square1, int square2) {
    struct AdjListNode* previous;
    struct AdjListNode* current;
    current = graph->array[square1].head;
    //If the node has no edges, return
    if (!current) {
        return;
    }
    //If node only has one edge and that edge is to be removed
    if (current->dest == square2) {
        struct AdjListNode* temp = current;
        graph->array[square1].head = current->next;
        free(temp);
        placeWallPiece(graph, square2, square1);
        return;
    } else {
        previous = current;
        current = current->next;
        while (current && previous) {
            if (current->dest == square2) {
                struct AdjListNode* temp = current;
                previous->next = current->next;
                free(temp);
                placeWallPiece(graph, square2, square1);
                return;
            }
            previous = current;
            current = current->next;
        }
    }
}

void removeWall(struct Graph* graph, int square1, int square2, int placement) {
    int edge;
    switch (placement) {
    case WALL_UP:
        edge = -dim;
        break;
    case WALL_DOWN:
        edge = dim;
        break;
    case WALL_RIGHT:
        edge = 1;
        break;
    case WALL_LEFT:
        edge = -1;
        break;
    }

    removeWallPiece(graph, square1, square1 + edge);
    removeWallPiece(graph, square1 + edge, square1);

    removeWallPiece(graph, square2, square2 + edge);
    removeWallPiece(graph, square2 + edge, square2);

}

void removeWallPiece(struct Graph* graph, int square1, int square2) {
    struct AdjListNode* newNode = newAdjListNode(square2);
    newNode->next = graph->array[square1].head;
    graph->array[square1].head = newNode;
}

//Returns a path but not the shortest path. Still needs work
int distanceToGoal(struct Graph* graph, int playerPos, int opponentPos, int player) {
    int distance[NUM_NODES + 2]; //Keeps track of distances to nodes
    int previous[NUM_NODES + 2]; //Previous node in optimal path from source
    int visited[NUM_NODES + 2] = {0}; //Tracks the nodes that have been visited
    int path[NUM_NODES + 2];
    int start, target, minDist, alt, temp;
    if (player == MAX_PLAYER) {
        start = playerPos; //Starting point is player position
        target = NUM_NODES + 1; //Player's target goal line
    } else {
        start = opponentPos; //Starting point is player position
        target = 0; //Player's target goal line
    }

    int i;
    int infinity = 1000; //Arbitrary Large value
    for (i = 0; i < NUM_NODES + 2; i++) {
        distance[i] = infinity;
        previous[i] = -1;
    }
    distance[start] = 0; //Distance to the starting point is 0
    visited[start] = 1; //Starting node marked as visited

    //While the target goal line is not reached
    while (visited[target] == 0) {
        minDist = infinity;
        temp = 0;

        struct AdjListNode* it = graph->array[start].head;
        while (it) {
            alt = distance[start] + 1;
            if (alt < distance[it->dest] && visited[it->dest] == 0) {
                distance[it->dest] = alt;
                previous[it->dest] = start;
                //temp = it->dest;
            }
            if (minDist > distance[it->dest] && visited[it->dest] == 0) {
                minDist = distance[it->dest];
                temp = it->dest;
            }
            it = it->next;
        }
        start = temp;
        visited[start] = 1;
    }
    start = target;
    int j = 0;
    while (start != -1) {
        path[j++] = start;
        start = previous[start];
    }

    for (j = j-1; j >= 0; j--) {
        printf("%d\t",path[j]);
    }
    return distance[target];

}

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
    int j, nextTurn;
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

double timer(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (((double) tv.tv_usec)/1e6);
}

int main(int argc, char* argv) {
    double begTree, endTree, endMiniMax;
    graph = createBoard();

    maxPlayer = (struct Player*) malloc(sizeof(struct Player));
    if (maxPlayer == NULL) {
        printf("\nMalloc failed Line 576\n");
        exit(0);
    }
    maxPlayer->num_walls = 10;
    maxPlayer->position = (int)sqrt(NUM_NODES)/2 + 1;
    maxPlayer->label = MAX_PLAYER;

    minPlayer = (struct Player*) malloc(sizeof(struct Player));
    if (minPlayer == NULL) {
        printf("\nMalloc failed Line 585\n");
        exit(0);
    }
    minPlayer->num_walls = 10;
    minPlayer->position = (NUM_NODES) - (int)sqrt(NUM_NODES)/2;
    minPlayer->label = MIN_PLAYER;

    int wall[3] = {-1, -1, -1};
    begTree = timer();
    struct TreeNode* gameTree = createGameTree(wall, DEPTH, MAX_PLAYER, maxPlayer->position, minPlayer->position);
    endTree = timer();
    int bestMove = minimax(gameTree, DEPTH, MAX_PLAYER);
    endMiniMax = timer();

    printf("\n\nBEST MOVE EVAL: %d\n\n", bestMove);

    //Printing Info to check if the correct nextState is returned
    /*
    printf("\n\nNext State Info: %d, %d, %d, %d, %d\n\n", nextState->maxPos, nextState->minPos, nextState->wall[0], nextState->wall[1], nextState->wall[2]);
    struct TreeNode* children = nextState->child;
    while(children) {
        if (!children->child) {
            printf("\nLEAF STATE: %d, %d, %d, %d, %d, %d\n\n", children->maxPos, children->minPos, children->wall[0], children->wall[1], children->wall[2], getEvaluation(children));
            children = children->sibling;
        } else {
            children = children->child;
        }

    } */

    //printRecursiveTree2(gameTree, 0);
    printf("DONE");

    printf("\nBuilding Tree -- Execution time : %.10e\n\n\n", endTree - begTree);
    printf("\nMiniMax -- Execution time : %.10e\n\n\n", endMiniMax - endTree);
    printf("\nTotal -- Execution time : %.10e\n\n\n", endMiniMax - begTree);
    //printGraph(graph);
}
