 /*
** server.c -- a stream socket server demo
*/
#include "agenda.h"
#include <sys/time.h>

/* Estrutura para analise de tempo em microsegundos */
struct timeval  first, second, lapsed;
struct timezone tzp; 


void sigchld_handler(int s)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
  int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;// TCP stream sockets
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
			 p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		   sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  while(1) {  // main accept() loop
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
	      get_in_addr((struct sockaddr *)&their_addr),
	      s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (!fork()) { // this is the child process
      menu(new_fd, their_addr);
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }

  return 0;
}

void cliTimeRecv(struct timeval first,struct timeval second, int tam){
  FILE * pFile;
  pFile = fopen("serverTimeRecvRTT.dat", "a"); /*arquivo com nome de usuarios*/

  if (pFile == NULL) 
    return ;
  
  if (first.tv_usec > second.tv_usec) { 
    second.tv_usec += 1000000; 
    second.tv_sec--; 
  } 
  
  fseek(pFile, 0, SEEK_END);
  fprintf(pFile,"Tempo de recebimento de um pacote de tam: %d e de %d microsegundos\n"
	  ,tam,second.tv_usec - first.tv_usec);
  fclose(pFile);

  return;
}

void menu(int new_fd, struct sockaddr_storage their_addr){
  User *user;
  char nome[20],senha[20],pwd[20],again[1];
  char str[65],buf[256]; 
  int numbytes;

  strcpy(str,"0123");//56 bytes + 8 de hearder

  while(1){

     recv(new_fd, buf, MAXDATASIZE-1, 0);
     send(new_fd, str , strlen(str), 0);

    }
    return;  
}


void sendStr(int new_fd, char str[]){
    perror("send");
}
