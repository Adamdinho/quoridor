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

extern int player;

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


  return 0;
}

/*(((((((((((((((((((((((((((((())))))))))))))))))))))))))))))
*
* \fn start_turn(char* last_turn)
* \param last_turn - a string of characters which encodes the last turn made by the opponent.
* \brief begins the process of evaluating moves and executing a turn.
**(((((((((((((((((((((((((((((())))))))))))))))))))))))))))))*/
void start_turn(char* last_turn) {


}
