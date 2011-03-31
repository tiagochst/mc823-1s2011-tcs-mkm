/*
** server.c -- a stream socket server demo
*/
#include "agenda.h"

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
      close(sockfd); // child doesn't need the listener
      // if (send(new_fd, "Hello, world!\n", 15, 0) == -1)
      //	perror("send");
      menu(new_fd, their_addr);
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
  }

  return 0;
}


void menu(int new_fd, struct sockaddr_storage their_addr){
  User *user;
  char nome[20];
  char str[1000] = "Escolha uma opcao:\n\
                  Opcao 1 - Entrar como um usuario\n\
                  Opcao 2 - Criar um usuario\n\
                  Opcao 3 - Sair\n";
    if (send(new_fd, str , strlen(str), 0) == -1)
      perror("send");
      printf("%d\n", strlen(str));
      switch(leOpcao(their_addr, new_fd)){
  case 1:
    /* Ler usuario */
    leNome(their_addr, new_fd,nome);
    break;
  case 2:
    /* Criar um usuario */
    leNome(their_addr, new_fd,nome);
    /* Verifica se nome ja existe */
    user=agenda_init(nome);
    break;
  default:
    return;
    break;
  }
}

void leNome(struct sockaddr_storage their_addr, int sockfd, char nome[]){

  int numbytes;
  char s[INET6_ADDRSTRLEN];
  socklen_t addr_len = sizeof their_addr;

    if ((numbytes = recvfrom(sockfd, nome, 256 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
   printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    /* buf[numbytes] = '\0'; */
    printf("listener: packet contains \"%s\"\n", nome);
    return;

}

int leOpcao(struct sockaddr_storage their_addr, int sockfd ){
  int numbytes;
  char buf[MAXDATASIZE];
  char s[INET6_ADDRSTRLEN];
  socklen_t addr_len = sizeof their_addr;

    if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
   printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
    printf("%c", buf[0]);
    return atoi(buf);
}
