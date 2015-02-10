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


struct TreeNode* newTreeNode2(struct Graph* state);
struct TreeNode* newTreeNode3(struct Graph* state, int type, int moveVal, int lev);
struct TreeNode* createGameTree(struct Graph* currentState, int depth, int turn);
struct TreeNode* newTreeNode(struct Graph* state);

#endif
