/*************************************************************
* \file game_state.c
* ECSE 456 Design Project
* Creation Date : 10/28/2014
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: Holds the state of the current game board.
**************************************************************/

#include "game_state.h"



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
    if (square1 <= 0 || square2 <= 0 || square1 > NUM_NODES || square2 > NUM_NODES) {
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
