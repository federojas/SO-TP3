#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levels.h"
#include <unistd.h>
#include <math.h>

#define FIRST_ASCII_PRINT 32
#define LAST_ASCII_PRINT 126
#define FILTER_MODULE 7
#define NORMAL_DIST_AMOUNT 1000

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static double getNormalDistributedNumber();
static double drand();
static int checkAnswer(FILE *clientFile, char *buffer, char *challengeAnswer);

int levelManager(FILE * clientFile, char * buffer, t_level * level) {
    printf("\n------------- DESAFIO -------------\n%s\n\n", level->challengeQuestion);
    if(level->challenge != NULL) {
        level->challenge();
    }
    printf("\n----- PREGUNTA PARA INVESTIGAR -----\n%s\n\n", level->investigationQuestion);
    return checkAnswer(clientFile, buffer, level->challengeAnswer);
}

void ebadfChallenge() {
    if (write(13, "................................La respuesta es fk3wfLCm3QvS\n", 62) == -1) {
        perror("write");
    }
}

void filterChallenge() {

    char * answer = "K5n2UFfpFMUN";
    int answerLength = strlen(answer);

    int i = 0;
    while (answer[i] != 0) {
        int randomfd = (rand() % FILTER_MODULE) + 1;

        if (randomfd == STDOUT_FILENO) {
            write(STDOUT_FILENO, answer[i++], 1);
        }

        else {
            char c = (char)(rand() % (FIRST_ASCII_PRINT - LAST_ASCII_PRINT + 1) + FIRST_ASCII_PRINT);
            write(STDERR_FILENO, &c, 1);
        }
    }
}

void incognitoChallenge() {
    printf("\033[30;40mLa respuesta es BUmyYq5XxXGt\033[0m\n\n");
}

void quineChallenge() {

    int quineCreatedFlag = system("gcc quine.c -o quine");

    if (quineCreatedFlag == 0) {
        int quineCorrectFlag;
        printf("¡Genial!, ya lograron meter un programa en quine.c, veamos si hace lo que corresponde.\n");
        quineCorrectFlag = system("./quine | diff - quine.c");
        if (quineCorrectFlag == 0) {
            printf("La respuesta es chin_chu_lan_cha\n\n");
        } else {
            printf("\ndiff encontró diferencias.\n");
        }
    } else { 
        printf("\n\n ENTER para reintentar.\n\n"); 
    }
    
}

void gdbChallenge() {
    if (getpid() == 0x12345678) {
        printf("La respuesta es: gdb_rules\n\n");
    }
}

void randomChallenge() {
    for (int i = 0; i < NORMAL_DIST_AMOUNT; i++) {
        printf("%.6f ", getNormalDistributedNumber());
    }  
}

static int checkAnswer(FILE *clientFile, char *buffer, char *challengeAnswer) {
    if(fgets(buffer, BUFFER_SIZE, clientFile) == NULL) {
        perror("fgets in levelManager failed");
        return -1;
    }
    return strcmp(buffer, challengeAnswer) == 0;
}

static double drand() {
  return (rand()+1.0)/(RAND_MAX+1.0);
}

static double getNormalDistributedNumber() {
  return sqrt(-2*log(drand())) * cos(2*M_PI*drand());
}