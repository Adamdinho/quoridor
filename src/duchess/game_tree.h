#ifndef GTREE_H
#define GTREE_H


#include "common_structs.h"
#include "duchess_init.h"

/*
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
};*/

struct TreeNode* newTreeNode(int maxPosition, int minPosition, int wall[3], int turn);
//struct TreeNode* newTreeNode2(struct Graph* state);
//struct TreeNode* newTreeNode3(struct Graph* state, int type, int moveVal, int lev);
struct TreeNode* createGameTree(int wall[3], int depth, int turn, int currentMaxPos, int currentMinPos);
//struct TreeNode* newTreeNode(struct Graph* state);

#endif
