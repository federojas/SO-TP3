#ifndef LEVELS_H
#define LEVELS_H

#include <stdio.h>

#define BUFFER_SIZE 50

typedef struct t_level{
    int level;
    char *challengeQuestion;
    char *challengeAnswer;
    char *investigationQuestion;
}t_level;

int levelManager(FILE * clientFile, char * buffer, char * challengeQuestion, char * challengeAnswer, char * investigationQuestion);
    
#endif