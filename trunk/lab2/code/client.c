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

#define PORT "35555" // the port client will be connecting to 

#define MAXDATASIZE 1000 // max number of bytes we can get at once 
char opcao[256]; 

/* Estrutura para analise de tempo em microsegundos */
struct timeval  first, second, lapsed;
struct timezone tzp;

void connecttime(struct timeval first,struct timeval second){

  double t1=first.tv_sec+(first.tv_usec/1000000.0); 
  double t4=second.tv_sec+(second.tv_usec/1000000.0); 

  FILE * pFile;
  pFile = fopen("conntime.dat", "a"); /*arquivo com tempos do servidor*/

  if (pFile == NULL) 
    return ;
  
  /* if (first.tv_usec > second.tv_usec) { 
    second.tv_usec += 1000000; 
    second.tv_sec--; 
  } */
 
  fseek(pFile, 0, SEEK_END);
  fprintf(pFile,"%f \n" ,t4-t1);
  fclose(pFile);

  return;
}


void clienteTimeRecv(struct timeval first,struct timeval second){

  double t1=first.tv_sec+(first.tv_usec/1000000.0); 
  double t4=second.tv_sec+(second.tv_usec/1000000.0); 

  FILE * pFile;
  pFile = fopen("clientTime.dat", "a"); /*arquivo com tempos do servidor*/

  if (pFile == NULL) 
    return ;
  
  /* if (first.tv_usec > second.tv_usec) { 
    second.tv_usec += 1000000; 
    second.tv_sec--; 
  } */
 
  fseek(pFile, 0, SEEK_END);
  fprintf(pFile,"%f \n" ,t4-t1);
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
    char s[INET6_ADDRSTRLEN]="",tempo[5],str[5];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
   gettimeofday (&first, &tzp); 
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
   gettimeofday (&second, &tzp); 
   connecttime(first,second);

    freeaddrinfo(servinfo); // all done with this structure
    int size;

    /* Teste de tempo */
   strcpy(str,"0123");//tamanho de um inteiro bytes
   gettimeofday (&first, &tzp); 
   send(sockfd, str , strlen(str), 0);
   
    while(1){
      /* Esperando resposta do servidor*/
      if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
	
      }

      system("clear");
      printf("\n%s\n",buf); //client received

      /* Espera resposta do servidor*/
      strcpy(opcao,"");
      scanf("%[^\n]", opcao );
      getchar();

      envia_pct(sockfd, opcao ,strlen(opcao) + 1);

      if(strcmp("q",opcao)==0){
	break;
      }
    }
 
    recv(sockfd, tempo, MAXDATASIZE-1, 0);
    gettimeofday (&second, &tzp); 
  
    clienteTimeRecv(first,second);
        
    close(sockfd);
    
    return 0;
}

   
