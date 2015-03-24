#ifndef QUQU
#define QUQU

#include <stdlib.h>
#include <stdio.h>

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

/*
struct TreeNode {
    struct Graph* state;
    struct QuoriQueue *children;
    int evaluation;
    int moveType; // 0 is root, 1 is pawn move, 2 is wall placement TODO enum this later
    int moveValue; // Destination if pawn move. will implement wall placements later.
    int level;
    //struct TreeNode* sibling;
};*/

struct BFTreeNode {
    struct TreeNode* nodeWrapped;
    int maxPos;
    int minPos;
};

struct QueueNode {
    struct BFTreeNode *thisNode;
    struct TreeNode *treeNode;
    struct QueueNode *nextNode;
};

struct QuoriQueue {
    struct QueueNode* head;
    struct QueueNode* tail;
    int length;
};

struct AdjListNode* newAdjListNode(int dest);

struct QuoriQueue* createQueue();

void enQueue(struct QuoriQueue* queue,struct BFTreeNode* toQueue);

struct BFTreeNode* deQueue(struct QuoriQueue* queue);

#endif
