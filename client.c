/**************************************************************************************/
/* @file    client_1.c                                                               */
/* @brief   This clients connects,                                                   */
/*          sends a text, reads what server and disconnects                          */
/*************************************************************************************/

#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_ADDRESS  "0.0.0.0"     /* server IP */
#define PORT            8080 
#define BUFF_LEN 125
/* Test sequences */
char buf_tx[BUFF_LEN];     //transmision, datos que envia el cliente al servidor

 
 
/* This clients connects, sends a text and disconnects */
int main() {
    int sockFd; //fd para el socket del cliente
    struct sockaddr_in servaddr;  //misma estructura que el servidor, para la direccion del sv, su puerto y su dominio
    
    char buf_rx[BUFF_LEN];

    /* Socket creation */
    sockFd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockFd == -1) { 
        perror("CLIENT: socket creation failed");
        exit(EXIT_FAILURE);
    } 
    
    
    //se setea en todos los bytes de la estructura un 0
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
    servaddr.sin_port = htons(PORT); 

    /* try to connect the client socket to server socket */
    /*
        primer argumento fd del socket creado para el cliente
        segundo la estructura con los campos
        tercer argumento el size de la estructura
        si devuelve un valor distinto de cero es que hubo un error
    */
    if (connect(sockFd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {  
        perror("CLIENT: connection with the server failed");
        exit(EXIT_FAILURE);
    } 
    
    printf("Connected to the server...\n"); 

    /*-------------BORRAR DESPUES-----------------*/
    char respuestas[][30]={"entendido\n", "itba\n","M4GFKZ289aku\n","fk3wfLCm3QvS\n", "too_easy\n", ".RUN_ME\n"};
    for(int i=0;i<6;i++) {
        write(sockFd,respuestas[i],strlen(respuestas[i]));
        
    }

    /*--------------------------------------------*/


    memset(buf_tx,0,BUFF_LEN);
    while(fgets(buf_tx,BUFF_LEN-1,stdin)!=NULL) {
        int len=write(sockFd,buf_tx,strlen(buf_tx));
        if(len<0) {
            perror("CLIENT: write failed");
            exit(EXIT_FAILURE);
        }
         memset(buf_tx,0,BUFF_LEN);
    }
    // /* send test sequences*/
    // write(sockFd, buf_tx, sizeof(buf_tx));     
    read(sockFd, buf_rx, sizeof(buf_rx));
    printf("CLIENT:Received: %s \n", buf_rx);
   
       
    /* close the socket */
    close(sockFd); 
} 