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

#define SERV_PORT       8080     
#define SERV_HOST_ADDR "0.0.0.0"     
#define BUF_SIZE        100              
#define BACKLOG         5      

static void startLevels(int clientfd);

static t_level levels[] = {
    {void, "Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\n\n
    En este TP deberán finalizar el juego que ya comenzaron resolviendo los desafíos de cada nivel.\n
    Además tendrán que investigar otras preguntas para responder durante la defensa.\n
    El desafío final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desafíos
    y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\n
    Además, deberán implementar otro programa para comunicarse conmigo.\n\n
    Deberán estar atentos a los easter eggs.\n\n
    Para verificar que sus respuestas tienen el formato correcto respondan a este desafío con la palabra 'entendido\\n'\n"
    ,"entendido\n", "¿Cómo descubrieron el protocolo, la dirección y el puerto para conectarse?\n\n"},
    {void, "The Wire S1E5\n5295 888 6288\n\n", "itba\n", 
    "¿Qué diferencias hay entre TCP y UDP y en qué casos conviene usar cada uno?\n\n"},
    {void, "https://ibb.co/tc0Hb6w\n\n", "M4GFKZ289aku\n", 
    "¿El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? ¿Por qué?\n\n"},
    {ebadfChallenge, "EBADF...\n\n", "fk3wfLCm3QvS\n", 
    "¿Qué útil abstracción es utilizada para comunicarse con sockets? 
    ¿se puede utilizar read(2) y write(2) para operar?\n\n"},
    {void, "respuesta = strings:245\n\n", "too_easy\n", 
    "¿Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?\n\n"},
    {void, ".data .bss .comment ? .shstrtab .symtab .strtab\n\n", ".RUN_ME\n", 
    "Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. 
    ¿Qué conviene más?\n\n"},
    {filterChallenge, "Filter error\n", "K5n2UFfpFMUN\n", 
    "¿Cómo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?\n\n"},
    {incognitoChallenge, "¿?\n\n", "BUmyYq5XxXGt\n", 
    "¿Qué aplicaciones se pueden utilizar para ver el tráfico por la red?\n\n"},
    {void, "Latexme\n\nSi\n\\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces\n
    y =\n\n", "u^v\n", "sockets es un mecanismo de IPC. ¿Qué es más eficiente entre sockets y pipes?\n\n"},
    {quineChallenge, "quine.\n\n", "chin_chu_lan_cha\n", "¿Cuáles son las características del protocolo SCTP?\n\n"},
    {gdbChallenge, "b gdbme y encontrá el valor mágico ENTER para reintentar.\n\n", "gdb_rules\n", "¿Qué es un RFC?\n\n"},
    {randomChallenge, "Me conoces\n\n", "normal\n", "¿Fue divertido?\n\n"}

};

int main(int argc, char* argv[]) {
    int serverfd, clientfd;  
       
    struct sockaddr_in servaddr;

    unsigned int servaddrLen = sizeof(servaddr), options = 1; 

    int  len_rx, len_tx = 0;  
    char buff_tx[BUF_SIZE]; 
    char buff_rx[BUF_SIZE];
    
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverfd == -1) { 
        perror("[SERVER-error]: socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options)) == -1) {
        perror("socket options settings failed\n");
        exit(0);
    }
  
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_port        = htons(SERV_PORT); 
    inet_aton(SERV_HOST_ADDR, &servaddr.sin_addr);
    
    if ((bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        perror("[SERVER-error]: socket bind failed")
        exit(EXIT_FAILURE);
    } 
  
    if ((listen(serverfd, BACKLOG)) != 0) { 
        perror("[SERVER-error]: socket listen failed")
        exit(EXIT_FAILURE);
    } 
  
    clientfd = accept(serverfd, (struct sockaddr *)&servaddr, (socklen_t*)&servaddrLen); 

    close(serverfd);

    if (clientfd < 0) { 
        perror("[SERVER-error]: connection not accepted");
        exit(EXIT_FAILURE);
    } 

    startLevels(clientfd);
                             
    close(clientfd);

    return 0;
}

// static void startLevels(int clientfd) {
//     printf("----------DESAFIO-------------\n");
//                 printf("%s\n",levels[0].challenge);

//             while(1) {  
//                 len_rx = read(clientfd, buff_rx, sizeof(buff_rx));  
                
//                 int respuesta=checkLevel(buff_rx,levels[0].challengeAnswer);
//                 printf("%d\n",respuesta);

//                 if(len_rx == -1) {
//                     perror("[SERVER-error]: clientfd cannot be read");
//                 }
//                 else if(len_rx == 0) {
//                     printf("[SERVER]: client socket closed \n\n");
//                     close(clientfd);
//                     break; 
//                 }
//                 else {
//                     write(clientfd, buff_tx, strlen(buff_tx));
//                     printf("[SERVER]: %s \n", buff_rx);
//                 }   
// }
        
                         
 