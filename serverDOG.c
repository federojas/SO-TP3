// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <unistd.h>
#include "levels.h"
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <time.h>

#define SERV_PORT       8080     
#define SERV_HOST_ADDR "0.0.0.0"                 
#define BACKLOG         5      
#define LEVELS_AMOUNT 12

static void startLevels(int clientfd);
static void clearScreen();
static int createServerSocket();
static void initializeServerSettings(struct sockaddr_in servaddr, int serverfd);
static void clientManager(struct sockaddr_in servaddr, int serverfd, unsigned int servaddrLen);

char * cow = " _______________________\n< ESTO ES UN EASTER_EGG >\n -----------------------\n        \\   ^__^\n         \\  (oo)\\_______\n            (__)\\       )\\/\\\n                ||----w |\n                ||     ||\n";

static 
t_level levels[] = {
    {NULL, "Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\n\nEn este TP deberán finalizar el juego que ya comenzaron resolviendo los desafíos de cada nivel.\nAdemás tendrán que investigar otras preguntas para responder durante la defensa.\nEl desafío final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desafíos y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\nAdemás, deberán implementar otro programa para comunicarse conmigo.\n\nDeberán estar atentos a los easter eggs.\n\nPara verificar que sus respuestas tienen el formato correcto respondan a este desafío con la palabra 'entendido\\n'"
    ,"entendido\n", "¿Cómo descubrieron el protocolo, la dirección y el puerto para conectarse?"},
    {NULL, "The Wire S1E5\n5295 888 6288", "itba\n", 
    "¿Qué diferencias hay entre TCP y UDP y en qué casos conviene usar cada uno?"},
    {NULL, "https://ibb.co/tc0Hb6w\n\n", "M4GFKZ289aku\n", 
    "¿El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? ¿Por qué?"},
    {&ebadfChallenge, "EBADF...", "fk3wfLCm3QvS\n", 
    "¿Qué útil abstracción es utilizada para comunicarse con sockets? ¿Se puede utilizar read(2) y write(2) para operar?"},
    {NULL, "respuesta = strings:85", "too_easy\n", 
    "¿Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?"},
    {NULL, ".data .bss .comment ? .shstrtab .symtab .strtab", ".RUN_ME\n", 
    "Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. ¿Qué conviene más?"},
    {&filterChallenge, "Filter error", "K5n2UFfpFMUN\n", 
    "¿Cómo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?"},
    {&incognitoChallenge, "¿?", "BUmyYq5XxXGt\n", 
    "¿Qué aplicaciones se pueden utilizar para ver el tráfico por la red?"},
    {NULL, "Latexme\n\nSi\\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces y ="
    ,"u^v\n", "sockets es un mecanismo de IPC. ¿Qué es más eficiente entre sockets y pipes?"},
    {&quineChallenge, "quine.", "chin_chu_lan_cha\n", 
    "¿Cuáles son las características del protocolo SCTP?"},
    {&gdbChallenge, "b gdbme y encontrá el valor mágico ENTER para reintentar.", "gdb_rules\n", 
    "¿Qué es un RFC?"},
    {&randomChallenge, "Me conoces", "normal\n", "¿Fue divertido?"}
};

int main(int argc, char* argv[]) {
    int serverfd;  
       
    struct sockaddr_in servaddr;

    serverfd = createServerSocket();

    initializeServerSettings(servaddr, serverfd);

    clientManager(servaddr, serverfd, sizeof(servaddr));
  
    return 0;
}

static void startLevels(int clientfd) {

    srand(time(0));

    FILE * clientFile = fdopen(clientfd, "r");

    if(clientFile == NULL) {
        perror("[SERVER-error]: client fdopen failed");
        return ;
    }

    char * input = malloc(sizeof(char) * BUFFER_SIZE);
    if(input == NULL) {
        fclose(clientFile);
        perror("[SERVER-error]: malloc failed");
        return ;
    }

    int levelCompleted = 0, levelIndex = 0;

    while (levelIndex < LEVELS_AMOUNT && levelCompleted != -1) {

        clearScreen();

        memset(input, 0, BUFFER_SIZE);

        levelCompleted = levelManager(clientFile, input, &levels[levelIndex]);

        if (levelCompleted  == 1) {
            levelIndex++;
        } else {
            printf("Respuesta incorrecta: %s\n", input);
            sleep(2);
        }
    }

    free(input);

    fclose(clientFile);

    if (levelCompleted == -1) {
        return ;
    }

    clearScreen();
    
    printf("Felicitaciones, finalizaron el juego. Ahora deberán implementar el servidor que se comporte como el servidor provisto\n\n");     
}   

static void clearScreen() {
    printf("\033[1;1H\033[2J");
}

static int createServerSocket() {
    int serverfd;
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    int options = 1;

    if (serverfd == -1) { 
        perror("[SERVER-error]: socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options)) == -1) {
        perror("[SERVER-error]: socket options settings failed\n");
        exit(0);
    }
    return serverfd;
}

static void initializeServerSettings(struct sockaddr_in servaddr, int serverfd) {
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(SERV_PORT); 
    if(inet_aton(SERV_HOST_ADDR, &servaddr.sin_addr) == 0) {
        perror("[SERVER-error]: inet_aton failed\n");
        exit(EXIT_FAILURE);
    }
    
    if ((bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        perror("[SERVER-error]: socket bind failed");
        exit(EXIT_FAILURE);
    } 
  
    if ((listen(serverfd, BACKLOG)) != 0) { 
        perror("[SERVER-error]: socket listen failed");
        exit(EXIT_FAILURE);
    } 
}
    
static void clientManager(struct sockaddr_in servaddr, int serverfd, unsigned int servaddrLen) {
    int clientfd = accept(serverfd, (struct sockaddr *)&servaddr, (socklen_t*) &servaddrLen); 

    close(serverfd);

    if (clientfd < 0) { 
        perror("[SERVER-error]: connection not accepted");
        exit(EXIT_FAILURE);
    } 

    startLevels(clientfd);
                             
    close(clientfd);
}
                         
 