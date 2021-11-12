#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

#define SERVER_ADDRESS  "0.0.0.0"     
#define PORT            8080 
#define BUFFER_SIZE 125

static int createClientSocket();
static int initializeClientSettings(struct sockaddr_in sockaddr, int clientfd);
void clientManager(struct sockaddr_in sockaddr, int clientfd);

int main(int argc, char const *argv[]) {
    int clientfd;
    struct sockaddr_in sockaddr;

    if(argc != 1) {
        fprintf(stderr, "Use ./client\n");
        exit(EXIT_FAILURE);
    }

    clientfd = createClientSocket(); 

    if(initializeClientSettings(sockaddr, clientfd) ==-1) {
        return -1;
    }
    
    clientManager(sockaddr, clientfd);
} 

static int createClientSocket() {
    int clientfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (clientfd == -1) { 
        perror("CLIENT: socket creation failed");
        exit(EXIT_FAILURE);
    } 
    return clientfd;
}

static int initializeClientSettings(struct sockaddr_in sockaddr, int clientfd) {
    if (inet_pton(AF_INET, SERVER_ADDRESS, &sockaddr.sin_addr) <= 0) {
        printf("CLIENT: inet_pton invalid address");
        return -1;
    }

    sockaddr.sin_family = AF_INET; 
    sockaddr.sin_port = htons(PORT); 

    if (connect(clientfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) != 0) {  
        perror("CLIENT: connection with the server failed");
        exit(EXIT_FAILURE);
    } 
}

void clientManager(struct sockaddr_in sockaddr, int clientfd) {
    printf("Connected to the server...\n"); 

    char buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);
    while(fgets(buffer, BUFFER_SIZE-1, stdin) != NULL) {
        int writeLength = write(clientfd, buffer, strlen(buffer));
        if(writeLength < 0) {
            perror("CLIENT: write failed");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(clientfd); 
}