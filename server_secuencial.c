#include <unistd.h>

/* sockets */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>

/* strings / errors*/
#include <errno.h>
#include <stdio.h> 
#include <string.h> 

/* server parameters */
#define SERV_PORT       8080              /* port */
#define SERV_HOST_ADDR "0.0.0.0"     /* IP, only IPV4 support  */
#define BUF_SIZE        100               /* Buffer rx, tx max size  */
#define BACKLOG         5                 /* Max. client pending connections  */

int main(int argc, char* argv[]) {
    int sockfd, connfd ;  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */ 
    //len se usara para guardar la longitud de la estructura "sockaddr_in" del cliente
    struct sockaddr_in servaddr, client; 
    /*para ipv4:
         struct sockaddr_in {
               sa_family_t    sin_family;  familia del socket 
               in_port_t      sin_port;   puerto
               struct in_addr sin_addr;   direccion
           };
    */

    int  len_rx, len_tx = 0;  /* received and sent length, in bytes */
    char buff_tx[BUF_SIZE] = "Hello client, I am the server"; //transmision
    char buff_rx[BUF_SIZE];   /* buffers for reception  */
    
    
    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //se guarda el fd, en el caso del server que nos dan es el 3
    if (sockfd == -1) 
    { 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully created..\n"); 
    }
    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr)); //inicializa con cero cada byte de esa estructura de adress format
  
    /* assign IP, SERV_PORT, IPV4 */
    servaddr.sin_family      = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); //convierte el string ese al formato de ip necesario
    servaddr.sin_port        = htons(SERV_PORT); //puerto definido, htons convierte el serv_port tenga la ordenacion de bytes que debe tener en la red
    
    
    /* Bind socket */ //asigna una direccion ip y un puerto al socket
    /*
    primer argumento es el fd que devolvio el socket 
    el segundo es un puntero a la estructura con los campos del server
    yy el ultimo es el tamaño de la estructura 
    */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully binded \n");
    }
  
    /* Listen */
    //hace que el socket creado se vuelva pasivo, esta escuchando a ver si hay conexiones entrantes 
    //usa el fd y backlog es el numero maximo de conexiones entrantes pendientes 
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Listening on SERV_PORT %d \n\n", ntohs(servaddr.sin_port) ); 
    }
    
    len = sizeof(client); 
  
      /* Accept the data from incoming sockets in a iterative way */
      while(1)
      {
          /*
            primer argumento el fd del socket 
            segundo argumento puntero a estructura del cliente, cuando accept retorne, en la estructura se nos cargaran los valores ip puerto y dominio del cliente que se ha conectado 
            puntero a len, que tiene el tamaño del struct 
          */
        connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
        if (connfd < 0) 
        { 
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
            return -1;
        } 
        else
        {              
            while(1) /* read data from a client socket till it is closed */ 
            {  
                /* read client message, copy it into buffer */
                /*
                    primer argumento fd del socket creado con el acept
                    segundo buffer de recepcion
                    tercero el size del buff
                */
                len_rx = read(connfd, buff_rx, sizeof(buff_rx));  
                
                if(len_rx == -1)
                {
                    fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                }
                else if(len_rx == 0) /* if length is 0 client socket closed, then exit */
                {
                    printf("[SERVER]: client socket closed \n\n");
                    close(connfd);
                    break; 
                }
                else
                {
                    //el read devolvio un valor mayor que cero entonces lo imprime en el buff de recepcion y ademas le manda algo con write
                    write(connfd, buff_tx, strlen(buff_tx));
                    printf("[SERVER]: %s \n", buff_rx);
                }            
            }  
        }                      
    } 
}