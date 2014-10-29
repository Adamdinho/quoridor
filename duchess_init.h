/*************************************************************
* File Name : duchess_init.h
* ECSE 456 Design Project
* Creation Date : 10/28/2014
* Author: Adam Pickersgill  <adam.pickersgill@mail.mcgill.ca>
*
* Description: header file for the initialization routine,
* designed modularly to be accessed by other modules.
**************************************************************/

#ifndef DUCHINIT_H
#define DUCHINIT_H

#include <stdlib.h> 
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


//TODO Extern this shit
int init(int p_1or2,int* eval_weights);
void start_turn(char* last_turn);

#endif
