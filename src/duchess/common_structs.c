
#include "common_structs.h"

struct AdjListNode* newAdjListNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
};


struct QuoriQueue* createQueue() {
    struct QuoriQueue* toReturn = (struct QuoriQueue*) malloc(sizeof(struct QuoriQueue));
    toReturn->length = 0;
    return toReturn;
}


void enQueue(struct QuoriQueue* queue,struct BFTreeNode* toQueue) {

    if (queue->length == 0) {
        struct QueueNode* new_node = (struct QueueNode*) malloc(sizeof(struct QueueNode));
        new_node->thisNode = toQueue;
        new_node->nextNode = NULL;
        queue->head = new_node;
        queue->tail = new_node;
        queue->length += 1;
    } else {
        struct QueueNode* new_node = (struct QueueNode*) malloc(sizeof(struct QueueNode));
        struct QueueNode* temp_node = queue->tail;

        new_node->thisNode = toQueue;
        new_node->nextNode = NULL;
        temp_node->nextNode = new_node;
        queue->tail = new_node;
        queue->length += 1;
    }

}


struct BFTreeNode* deQueue(struct QuoriQueue* queue) {

    struct BFTreeNode* to_return;

    if (queue->length == 0) {
        printf ("ERROR: No values in queue at %p", queue);
        return NULL;
    } else if (queue->length == 1){
        to_return = queue->head->thisNode;
        free(queue->tail);
        queue->head = NULL;
        queue->tail = NULL;
        queue->length -= 1;
    } else {
        to_return = queue->head->thisNode;
        struct QueueNode* node = queue->head;
        queue->head = node->nextNode;
        free(node);
        queue->length -= 1;
    }

    return to_return;

}
