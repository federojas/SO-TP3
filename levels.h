#ifndef LEVELS_H
#define LEVELS_H

#include <stdio.h>

typedef struct t_level{
    int level;
    char *challengeQuestion;
    char *challengeAnswer;
    char *investigationQuestion;
}t_level;

int checkLevel(char *usrAnswr, char *challengeAnswer);
    
#endif