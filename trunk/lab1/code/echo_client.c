/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT "34900" // the port client will be connecting to 

#define MAXDATASIZE 1000 // max number of bytes we can get at once 
char opcao[65]; 

/* Estrutura para analise de tempo em microsegundos */
struct timeval  first, second, lapsed;
struct timezone tzp; 


void cliTimeRecv(struct timeval first,struct timeval second, int tam){
  FILE * pFile;
  pFile = fopen("cliTimeRecvRTT.dat", "a"); /*arquivo com nome de usuarios*/

  if (pFile == NULL) 
    return ;
  
  if (first.tv_usec > second.tv_usec) { 
    second.tv_usec += 1000000; 
    second.tv_sec--; 
  } 
  
  fseek(pFile, 0, SEEK_END);
  fprintf(pFile,"%d\n" ,second.tv_usec - first.tv_usec);
  fclose(pFile);

  return;
}


void envia_pct( int sockfd, char s[], int size){
  if (( send(sockfd, s ,size, 0)) == -1) {
    perror("talker: sendto");
    exit(1);
  }
 return;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE]="";
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN]="";

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    int size;

      strcpy(opcao,"1");//pedido de echo

    while(1){

      /*Contagem de tempo para receber pacote*/
      gettimeofday (&first, &tzp); 
      envia_pct(sockfd, opcao ,strlen(opcao));
      /* Esperando resposta do servidor*/
      recv(sockfd, buf, MAXDATASIZE-1, 0);
      gettimeofday (&second, &tzp); 
      cliTimeRecv(first,second,strlen(buf));

    }
    
    close(sockfd);
    
    return 0;
}

   
