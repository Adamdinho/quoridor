/*************************************************************
* \file quoridojo.c
* ECSE 456 Design Project
* Creation Date : 10/29/2014
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: Currently a stub class to test integrity of the
* build process.
**************************************************************/
#include "quoridojo.h"

int p1,p2;

int main(int argc, char** argv) {
    int idx = 0;
    int game = 0;
    int tourney = 0;
    int evol = 0;
    int fitness = 0;

    for (idx = 1; idx < argc;  idx++) {
       if (strcmp(argv[idx], "-g") == 0) {
          game = 1;
       } else if (strcmp(argv[idx], "-t") == 0) {
          if (game == 1) {
            printf("Only one flag can be used at a time \n");
            return -1;
          }
          tourney = 1;
       } else if (strcmp(argv[idx], "-e") == 0) {
          if (game == 1 || tourney == 1) {
            printf("Only one flag can be used at a time \n");
            return -1;
          }
          evol = 1;
       } else if (strcmp(argv[idx], "-p") == 0) {
          if (game == 1  || tourney == 1  || evol == 1) {
            printf("Only one flag can be used at a time \n");
            return -1;
          }
          fitness = 1;
       } else {

       }
    }

    if (game == 1) {

        // Redo this so that Init sets up the game situation, but each turn tells the agent who it is. TODO
        init(1, &p1);
        init(2, &p2);

        begin_game(1,2);
    } else if (tourney == 1) {

    } else if (evol == 1) {

    } else if (fitness == 1) {

    }

    return 0;
}
