#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levels.h"

int checkLevel(char *usrAnswr, char *challengeAnswer){
    return strcmp(usrAnswr,challengeAnswer)==0;
}