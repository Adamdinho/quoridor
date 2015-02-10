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
