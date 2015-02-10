#ifndef CMOW_H
#define CMOW_H

#include "duchess_init.h"


int getEvaluation(struct TreeNode* node);
int minimax(struct TreeNode* node, int depth, int player);

#endif
