#ifndef LEVELS_H
#define LEVELS_H

#include <stdio.h>

#define BUFFER_SIZE 100    

typedef void (* t_challenge_function )(void);

typedef struct t_level{
    t_challenge_function challenge;
    char *challengeQuestion;
    char *challengeAnswer;
    char *investigationQuestion;
}t_level;

int levelManager(FILE * clientFile, char * buffer, t_level * level);
void ebadfChallenge();
void filterChallenge();
void incognitoChallenge();
void quineChallenge();
void gdbChallenge();
void randomChallenge();
#endif