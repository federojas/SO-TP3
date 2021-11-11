#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levels.h"

int checkLevel(char *usrAnswr, char *challengeAnswer){
    return strcmp(usrAnswr,challengeAnswer)==0;
}

int levelManager(FILE * clientFile, char * buffer, char * challengeQuestion, char * challengeAnswer, char * investigationQuestion) {
    printf("%s\n", challengeQuestion);

    printf("%s\n", investigationQuestion);
    return inputAnalyzer(clientFile, buff, ans);
}