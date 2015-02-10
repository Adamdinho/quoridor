/*************************************************************
* \file duchess_init.c
* ECSE 456 Design Project
* Creation Date : 10/28/2014
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: Contains the logic to initialize the agent and
* initiate a turn during the the running of a quoridojo
* training exercise.
**************************************************************/
#include "duchess_init.h"
#include "game_tree.h"

extern int player;
struct TreeNode* gameTree;
struct Player* minPlayer;
struct Graph* graph;
struct Player* maxPlayer;

struct Graph* createBoard(struct Graph* graph) {

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

double timer(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (((double) tv.tv_usec)/1e6);
}


/*(((((((((((((((((((((((((((((())))))))))))))))))))))))))))))
*
* \fn int init(p_1or2,eval_weights)
* \param p_1or2 which is an integer indicating which player this instance of duchess represents in the current game
* \param eval_weights an array of integers which holds the evaluation weights that define this instance of duchess
* \brief initializes this instance of duchess and provides methods to be accessed by external packages.
* \return 0 on success -1 otherwise.
**(((((((((((((((((((((((((((((())))))))))))))))))))))))))))))*/
int init(int p_1or2,int* eval_weights) {
  //player = p_1or2;


  minPlayer = (struct Player*) malloc(P_SIZE);
  graph = (struct Graph*) malloc(G_SIZE);
  maxPlayer = (struct Player*) malloc(P_SIZE);

  graph = createBoard(graph);

  maxPlayer->num_walls = 10;
  maxPlayer->position = (int)sqrt(NUM_NODES)/2 + 1;
  maxPlayer->label = MAX_PLAYER;

  minPlayer->num_walls = 10;
  minPlayer->position = (NUM_NODES) - (int)sqrt(NUM_NODES)/2;
  minPlayer->label = MIN_PLAYER;

  graph->player = maxPlayer;
  graph->opponent = minPlayer;

  //begTree = timer();
  gameTree = createGameTree(graph, DEPTH, MAX_PLAYER);
  //endTree = timer();

  return 0;
}

/*(((((((((((((((((((((((((((((())))))))))))))))))))))))))))))
*
* \fn start_turn(char* last_turn)
* \param last_turn - a string of characters which encodes the last turn made by the opponent.
* \brief begins the process of evaluating moves and executing a turn.
**(((((((((((((((((((((((((((((())))))))))))))))))))))))))))))*/
void start_turn(char* last_turn) {
  double begTree, endTree, endMiniMax;


  //TODO Update Tree(s)

  int bestMove = minimax(gameTree, DEPTH, MAX_PLAYER);
  //endMiniMax = timer();

  printf("\n\nBEST MOVE EVAL: %d\n\n", bestMove);

  //printTree(gameTree);

  //printf("\nBuilding Tree -- Execution time : %.10e\n\n\n", endTree - begTree);
  //printf("\nMiniMax -- Execution time : %.10e\n\n\n", endMiniMax - endTree);
  //printf("\nTotal -- Execution time : %.10e\n\n\n", endMiniMax - begTree);
  //printGraph(graph);

}
