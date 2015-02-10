#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>

#define MAX_PLAYER 1
#define MIN_PLAYER 0
#define NUM_NODES 81
#define DEPTH 6
#define NUM 20

#define WALL_UP 0
#define WALL_DOWN 1
#define WALL_LEFT 2
#define WALL_RIGHT 3

#undef max
#undef min

static int const dim = sqrt(NUM_NODES);
struct Player* maxPlayer;
struct Player* minPlayer;
struct Graph* graph;
double numbers[10] = {2.564,8.546,25.548,46.54897,24.56576,22.25616,46.8876,100.528,884.248,94.58};

struct Player {
    int position;
    int num_walls;
    int label;
};

struct TreeNode {
    struct Graph* state;
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
    struct Player* player;
    struct Player* opponent;
};

struct AdjListNode* newAdjListNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
};

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

struct Graph* createBoard() {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

    graph->array = (struct AdjList*) malloc((NUM_NODES + 2) * sizeof(struct AdjList));

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
int placeWall(struct Graph* graph, int square1, int square2, int placement) {
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
            }
        } else if (placement == WALL_DOWN) {
            if (isValidWall(square1, square1 + dim) &&
                isValidWall(square2, square2 + dim)) {
                    placeWallPiece(graph, square1, square1 + dim);
                    placeWallPiece(graph, square2, square2 + dim);
                    return 1;
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
            }
        } else if (placement == WALL_RIGHT) {
            if (isValidWall(square1, square1 + 1) &&
                isValidWall(square2, square2 + 1)) {
                    placeWallPiece(graph, square1, square1 + 1);
                    placeWallPiece(graph, square2, square2 + 1);
                    return 1;
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

//Returns a path but not the shortest path. Still needs work
int distanceToGoal(struct Graph* graph, int player) {
    int distance[NUM_NODES + 2]; //Keeps track of distances to nodes
    int previous[NUM_NODES + 2]; //Previous node in optimal path from source
    int visited[NUM_NODES + 2] = {0}; //Tracks the nodes that have been visited
    int path[NUM_NODES + 2];
    int start, target, minDist, alt, temp;
    if (player == MAX_PLAYER) {
        start = graph->player->position; //Starting point is player position
        target = NUM_NODES + 1; //Player's target goal line
    } else {
        start = graph->opponent->position; //Starting point is player position
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

void printTree(struct TreeNode* tree) {
    struct TreeNode* child = tree;
    struct TreeNode* children = tree->child;
    struct TreeNode* siblings = tree;
    struct TreeNode* node = tree;

    printf("ROOT:\t\tPlayer: %d\tOpponent: %d\n\n", tree->state->player->position, tree->state->opponent->position);
    printf("Child1:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->state->player->position, tree->child->state->opponent->position);
    printf("Child2:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->state->player->position, tree->child->sibling->state->opponent->position);
    printf("Child3:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->sibling->state->player->position, tree->child->sibling->sibling->state->opponent->position);

    printf("Child4:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->child->state->player->position, tree->child->child->state->opponent->position);
    printf("Child5:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->child->sibling->state->player->position, tree->child->child->sibling->state->opponent->position);
    printf("Child6:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->child->sibling->sibling->state->player->position, tree->child->child->sibling->sibling->state->opponent->position);

    printf("Child7:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->child->state->player->position, tree->child->sibling->child->state->opponent->position);
    printf("Child8:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->child->sibling->state->player->position, tree->child->sibling->child->sibling->state->opponent->position);
    printf("Child9:\t\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->child->sibling->sibling->state->player->position, tree->child->sibling->child->sibling->sibling->state->opponent->position);

    printf("Child10:\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->sibling->child->state->player->position, tree->child->sibling->sibling->child->state->opponent->position);
    printf("Child11:\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->sibling->child->sibling->state->player->position, tree->child->sibling->sibling->child->sibling->state->opponent->position);
    printf("Child12:\tPlayer: %d\tOpponent: %d\n\n", tree->child->sibling->sibling->child->sibling->sibling->state->player->position, tree->child->sibling->sibling->child->sibling->sibling->state->opponent->position);

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
    return node->state->player->position*node->state->opponent->position;
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
    maxPlayer->num_walls = 10;
    maxPlayer->position = (int)sqrt(NUM_NODES)/2 + 1;
    maxPlayer->label = MAX_PLAYER;

    minPlayer = (struct Player*) malloc(sizeof(struct Player));
    minPlayer->num_walls = 10;
    minPlayer->position = (NUM_NODES) - (int)sqrt(NUM_NODES)/2;
    minPlayer->label = MIN_PLAYER;

    graph->player = maxPlayer;
    graph->opponent = minPlayer;

    begTree = timer();
    struct TreeNode* gameTree = createGameTree(graph, DEPTH, MAX_PLAYER);
    endTree = timer();
    int bestMove = minimax(gameTree, DEPTH, MAX_PLAYER);
    endMiniMax = timer();

    printf("\n\nBEST MOVE EVAL: %d\n\n", bestMove);

    //printTree(gameTree);

    printf("\nBuilding Tree -- Execution time : %.10e\n\n\n", endTree - begTree);
    printf("\nMiniMax -- Execution time : %.10e\n\n\n", endMiniMax - endTree);
    printf("\nTotal -- Execution time : %.10e\n\n\n", endMiniMax - begTree);
    //printGraph(graph);
}
