#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levels.h"

static int checkAnswer(FILE *clientFile, char *buffer, char *challengeAnswer){
    if(fgets(buffer, BUFFER_SIZE, clientFile) == NULL) {
        perror("fgets in levelManager failed");
        return -1;
    }
    return strcmp(buffer, challengeAnswer) == 0;
}

int levelManager(FILE * clientFile, char * buffer, char * challengeQuestion, char * challengeAnswer, char * investigationQuestion) {
    printf("\n------------- DESAFIO -------------\n%s\n\n", challengeQuestion);

    printf("\n----- PREGUNTA PARA INVESTIGAR -----\n%s\n\n", investigationQuestion);
    return checkAnswer(clientFile, buffer, challengeAnswer);
}