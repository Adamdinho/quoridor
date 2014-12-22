#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "QuoriQueue.h"
#include <mpi.h>
#include <limits.h>

#define MAX_PLAYER 1
#define MIN_PLAYER 0
#define NUM_NODES 81
#define DEPTH 10
#define NUM 20

#define WALL_UP 0
#define WALL_DOWN 1
#define WALL_LEFT 2
#define WALL_RIGHT 3

#undef max
#undef min

struct Player* maxPlayer;
struct Player* minPlayer;
struct Graph* graph;
int rank, threads;
struct BFTreeNode* game_tree_root;
double numbers[10] = {2.564,8.546,25.548,46.54897,24.56576,22.25616,46.8876,100.528,884.248,94.58};



struct AdjListNode* newAdjListNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
}

struct TreeNode* newTreeNode2(struct Graph* state) {
    struct TreeNode* newNode = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    newNode->state = state;
    newNode->children = NULL;
}

struct TreeNode* newTreeNode(struct Graph* state, int type, int moveVal, int lev) {
    struct TreeNode* newNode = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    newNode->state = state;
    newNode->children = createQueue();
    newNode->moveType = type;
    newNode->moveValue = moveVal;
    newNode->level = lev;
    newNode->evaluation = NULL;
}

struct Graph* createBoard() {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

    graph->array = (struct AdjList*) malloc((NUM_NODES + 2) * sizeof(struct AdjList));

    int i, j;
    for (i = 0; i < NUM_NODES + 2; ++i) {
        graph->array[i].head = NULL;
    }

    int dim = sqrt(NUM_NODES);
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


//Checks is wall placement is valid. If so wall is placed, else returns.
//Code can be cleaner. Will modify later
int placeWall(struct Graph* graph, int square1, int square2, int placement) {
    int dim = sqrt(NUM_NODES);
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

// Used to create a single node, storing all of the appropriate status variables when called by the tree creator.

struct BFTreeNode* createChild(int curPos, int toPos, int maxmin, int level, int oppCurPos) {
    struct BFTreeNode* toReturn = malloc(sizeof(struct BFTreeNode));
    struct TreeNode* createdNode = newTreeNode(graph,0,toPos-curPos,level);
    toReturn->nodeWrapped = createdNode;

    // Depends upon whether the row is a min or max row
    if (maxmin == 0) {
        toReturn->maxPos = toPos;
        toReturn->minPos = oppCurPos;
    } else {
        toReturn->minPos = toPos;
        toReturn->maxPos = oppCurPos;
    }
    return toReturn;
}

// Serially creates the game tree (or subtree) rooted at the node passed in, to the depth specified.
// Maxmin is used to indicate whether a max or min player starts at the root
struct BFTreeNode* createGameTree(struct BFTreeNode* root, int depth, int maxmin) {
    struct QuoriQueue* queue = createQueue();

    int currentLevel = 0, total = 0, procs_assigned = 0;

    // EnQueue the root
    enQueue(queue,root);

    struct BFTreeNode* current;
    int position = 0, opp = 0;

    // Prepare the first node to be operated on
    current = deQueue(queue);
    currentLevel = current->nodeWrapped->level;

    do {
	// Set the parameters depending on if it is a max or min level
        if (currentLevel%2 == 0) {
          position = current->maxPos;
          opp = current->minPos;
          maxmin = 0;
        } else {
          position = current->minPos;
          opp = current->maxPos;
          maxmin = 1;
        }

        struct BFTreeNode* newChild;

	// Create a child for moving right	
	// (Assuming that is a valid move)
        if (isValidMove(position,position+1)) {
            newChild = createChild(position,position+1,maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
        }

	// Create a child for moving left	
	// (Assuming that is a valid move)
        if (isValidMove(position,position-1)) {
            newChild = createChild(position,position-1,maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
        }

	// Create a child for moving forward	
	// (Assuming that is a valid move)
        if (isValidMove(position,position+(int) sqrt(NUM_NODES))) {
            newChild = createChild(position,position+(int) sqrt(NUM_NODES),maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
        }

	// Create a child for moving backward	
	// (Assuming that is a valid move)
        if (isValidMove(position,position-(int) sqrt(NUM_NODES))) {
            newChild = createChild(position,position-(int) sqrt(NUM_NODES),maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
        }

	// Continue until you reach the depth desired
       	current = deQueue(queue);
        currentLevel = current->nodeWrapped->level;
    } while (currentLevel < depth);

    free(queue);

    return root;
}

// Decomposes and assigns subtrees to the various processes
// Stores the pointers to the subtree roots in temp_array
void createGameTreeParallel(struct Graph* currentState, int depth, int procs, struct BFTreeNode** temp_array) {
    struct QuoriQueue* queue;

    struct TreeNode* root = newTreeNode(currentState,0,NULL,0);
    queue = createQueue();

    // Initialize the root of the tree
    struct BFTreeNode* BFroot = (struct BFTreeNode*) malloc(sizeof(struct BFTreeNode));
    BFroot->nodeWrapped = root;
    BFroot->maxPos = (int)sqrt(NUM_NODES)/2 + 1;
    BFroot->minPos = (NUM_NODES) - (int)sqrt(NUM_NODES)/2;

    game_tree_root = BFroot;

    int currentLevel = 0, total = 0, procs_assigned = 0;

    enQueue(queue,BFroot);

    struct BFTreeNode* current;
    int position = 0, opp = 0;
    int maxmin = 0; //MAX is 0 MIN is 1

    // Building the small part of the tree that is built by every process to reference
    while (total < procs) {
        current = deQueue(queue);
        currentLevel = current->nodeWrapped->level;
        if (currentLevel%2 == 0) {
          position = current->maxPos;
          opp = current->minPos;
          maxmin = 0;
        } else {
          position = current->minPos;
          opp = current->maxPos;
          maxmin = 1;
        }

        struct BFTreeNode* newChild;

	// Use the same functions as in the serial version, using a BFS

        if (isValidMove(position,position+1)) {
            newChild = createChild(position,position+1,maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
            total += 1;
        }

        if (isValidMove(position,position-1)) {
            newChild = createChild(position,position-1,maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
            total += 1;
        }

        if (isValidMove(position,position+(int) sqrt(NUM_NODES))) {
            newChild = createChild(position,position+(int) sqrt(NUM_NODES),maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
            total += 1;
        }

        if (isValidMove(position,position-(int) sqrt(NUM_NODES))) {
            newChild = createChild(position,position-(int) sqrt(NUM_NODES),maxmin,currentLevel+1,opp);
            enQueue(queue,newChild);
            enQueue(current->nodeWrapped->children,newChild);
            total += 1;
        }
        total -= 1;


    }

    // Assign the subtree roots to each process
    while (procs_assigned < procs - 1) {
        temp_array[procs_assigned] = deQueue(queue);
        procs_assigned++;
    }

    // Handle the special case where some process needs to hold mutiple subtrees (because the root is in the universal space)
    if (rank == procs-1) {
        struct BFTreeNode* node;
        int x = 0;
        while (queue->length > 0) {
            node = deQueue(queue);
            //printf("Node = level %d, from %d, move %d, num children %d, pid %d \n",node->nodeWrapped->level,node->minPos,node->maxPos,node->nodeWrapped->children->length,rank);

            node = createGameTree(node,depth,node->nodeWrapped->level%2);
            temp_array[rank+x] = node;
            x++;
        }
        if (x < 3) {
          for (x = x; x < 3; x++) {
            temp_array[rank+x] = NULL;
          }
        }

    } else {
        temp_array[rank] = createGameTree(temp_array[rank],depth,temp_array[rank]->nodeWrapped->level%2);
    }

    free(queue);
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

int max(int a, int b) {
    return (a > b ? a : b);
}

int min(int a, int b) {
    return (a < b ? a : b);
}

// This is where the major processing at each node occurs.
// Commented code here was used for stress testing purposes, it doesn't calculate anything useful
// At this point it is a very bare bones evaluation, as the project progresses the evaluation functions will be more
// comprehensive, but also more complex and computationally expensive.
int getEvaluation(struct BFTreeNode* node) {
	/*int i,j,x;
	for (x = 0; x < NUM; x++) {
	for (i = 0; i < 10; i++) {
		for (j = 0; j <10; j++) {
			numbers[i] = numbers[i]*numbers[j] - numbers[j]/numbers[i];
		}
	}
	}*/
    return node->minPos*node->maxPos;
}

//Minimax, performs the minimax algorithm from the node down to the depth specified
int minimax(struct BFTreeNode* node, int depth, int player) {
    int value;
    struct QueueNode* child;

    //THE FOLLOWING PART WONT WORK FOR CONSECUTIVE TURNS... (but maybe that's moot?)
    if (node->nodeWrapped->evaluation != NULL ) {
        return node->nodeWrapped->evaluation;
    }

    // Return if at the bottom.
    if (depth == 0 || node->nodeWrapped->children->length == 0) {
        node->nodeWrapped->evaluation = getEvaluation(node);
        return node->nodeWrapped->evaluation;
    }

    // Recursively call and pass up the max or the min dependent on the player
    if (player == MAX_PLAYER) {
        int bestValue = INT_MIN;
        child = node->nodeWrapped->children->head;
        while (child) {
            value = minimax(child->thisNode, depth - 1, MIN_PLAYER);
            bestValue = max(bestValue, value);
            child = child->nextNode;
        }
        node->nodeWrapped->evaluation = bestValue;
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        child = node->nodeWrapped->children->head;
        while (child) {
            value = minimax(child->thisNode, depth - 1, MAX_PLAYER);
            bestValue = min(bestValue, value);
            child = child->nextNode;
        }
        node->nodeWrapped->evaluation = bestValue;
        return bestValue;
    }
}

// Free memory
void freeNode(struct BFTreeNode* node) {
    free(node->nodeWrapped->children);
    free(node->nodeWrapped);
    free(node);

}

// Helper method for memory management
void releaseTree(struct BFTreeNode* node) {
    int value;
    struct QueueNode* child;

    if (node->nodeWrapped->children->length == 0) {
        freeNode(node);
    }

    while (node->nodeWrapped->children->length > 0) {
        child = deQueue(node->nodeWrapped->children);
        releaseTree(child->thisNode);
    }
}

double timer(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (((double) tv.tv_usec)/1e6);
}

int main(int argc, char** argv) {
    
    // Initialize everything
    double begTree, endTree, endMiniMax;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &threads);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int i;

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


    struct BFTreeNode *head_nodes[threads+3];

    begTree = timer();

    // Create the game tree
    createGameTreeParallel(graph,DEPTH,threads, head_nodes);

    endTree = timer();

    int proc_result, total_result;
    MPI_Status status;

	// Handle the special case of multiple subtrees
    if (rank == threads-1) {
        int x = 0;
        for (x = 0; x < 3; x++) {
          if (head_nodes[rank+x] != NULL ){

            if (head_nodes[rank+x]->nodeWrapped->level % 2 == 0) {
                head_nodes[rank+x]->nodeWrapped->evaluation = minimax(head_nodes[rank+x],DEPTH-head_nodes[rank+x]->nodeWrapped->level,MAX_PLAYER);
            } else {
                head_nodes[rank+x]->nodeWrapped->evaluation = minimax(head_nodes[rank+x],DEPTH-head_nodes[rank+x]->nodeWrapped->level,MIN_PLAYER);
            }
          }
        }

	// Receive the minimax results to include in the master tree
        for (i = 0; i < threads-1; i++) {
            MPI_Recv(&head_nodes[i]->nodeWrapped->evaluation,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
        }
	
	// Calculate the master tree
        total_result = minimax(game_tree_root,DEPTH-head_nodes[rank]->nodeWrapped->level,MAX_PLAYER);
        printf(" Final Evaluation is : %d \n",total_result);

    } else {
	// Produce results for any processors that only have one subtree
        if (head_nodes[rank]->nodeWrapped->level % 2 == 0) {
            proc_result = minimax(head_nodes[rank],DEPTH-head_nodes[rank]->nodeWrapped->level,MAX_PLAYER);
        } else {
            proc_result = minimax(head_nodes[rank],DEPTH-head_nodes[rank]->nodeWrapped->level,MIN_PLAYER);
        }
	// Send results to the final process
        MPI_Send(&proc_result,1,MPI_INT,threads-1,0,MPI_COMM_WORLD);
    }
    endMiniMax = timer();

    // Display final timing information
    printf("\nBuilding Tree -- Execution time : %.10e\n", endTree - begTree);
    printf("\nMiniMax -- Execution time : %.10e\n", endMiniMax - endTree);
    printf("\nTotal -- Execution time : %.10e\n", endMiniMax - begTree);


    //releaseTree(head_nodes[rank]);
    //releaseTree(game_tree_root);

    //printGraph(graph);

    free(maxPlayer);
    free(minPlayer);
    free(graph);


    MPI_Finalize();
}
