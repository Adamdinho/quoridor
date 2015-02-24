/*************************************************************
* \file game_room.c
* ECSE 456 Design Project
* Creation Date : 2/06/2015
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: Currently a stub class to test integrity of the
* build process.
**************************************************************/
#include "game_room.h"

struct game_result {
  int winnerID;
  int loserID;
  int turns;
  int walls_played1;
  int walls_played2;
  int distance_from_goal_loser;
};


int turn_token;

void record_winner() {

}

struct game_result* determine_results() {
  struct game_result* result;
  result = (struct game_result*) malloc(sizeof(struct game_result));


  return result;
}

int begin_game(int id1, int id2) {
    turn_token = 0;
    char turn_tokens;
    struct game_result* result_of_game;
    while (is_game_complete() == 0) {

      if (turn_token % 2 == 0) {
        start_turn(1,&turn_tokens);
      } else {
        start_turn(2,&turn_tokens);
      }

      turn_token++;
    }

    result_of_game = determine_results();

    record_winner();
    printf(" Winner amd such \n");
    printf(" Number of turns in this game: %d \n", turn_token);

    free(result_of_game);

    return 0;
}
