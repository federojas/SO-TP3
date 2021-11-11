#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_ADDRESS_DEFAULT  "0.0.0.0"     
#define PORT_DEFAULT            8080 
#define BUFFER_SIZE 125
char buf_tx[BUFFER_SIZE];

int main(int argc, char const *argv[]) {
    int sockFd;
    struct sockaddr_in sockaddr;
    
    char buf_rx[BUFFER_SIZE];

    int port = PORT_DEFAULT;

    if(argc != 1 && argc != 3) {
        fprintf(stderr, "Use ./client to use default address and port settings, or ./client <address> <port> for custom options.\n");
        exit(EXIT_FAILURE);
    }

    if(argc == 3) {
        port = atoi(argv[2]);
    }

    sockFd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockFd == -1) { 
        perror("CLIENT: socket creation failed");
        exit(EXIT_FAILURE);
    } 

    if (inet_pton(AF_INET, argc == 3 ? argv[1] : SERVER_ADDRESS_DEFAULT, &sockaddr.sin_addr) <= 0) {
        printf("CLIENT: inet_pton invalid address");
        return -1;
    }

    sockaddr.sin_family = AF_INET; 
    sockaddr.sin_port = htons(port); 

    if (connect(sockFd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) != 0) {  
        perror("CLIENT: connection with the server failed");
        exit(EXIT_FAILURE);
    } 
    
    printf("Connected to the server...\n"); 

    memset(buf_tx, 0, BUFFER_SIZE);
    while(fgets(buf_tx, BUFFER_SIZE-1, stdin) != NULL) {
        int writeLength = write(sockFd, buf_tx, strlen(buf_tx));
        if(writeLength < 0) {
            perror("CLIENT: write failed");
            exit(EXIT_FAILURE);
        }
        memset(buf_tx, 0, BUFFER_SIZE);
    }

    close(sockFd); 
} 