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
  char nome[20],senha[20],pwd[20],again[1];
  char str[1000];
  while(1){
    sendStr(new_fd,"Escolha uma opcao:\n\
                  Opcao 1 - Entrar como um usuario\n\
                  Opcao 2 - Criar um usuario\n\
                  Opcao q - Sair\n\0");
    switch(leOpcao(their_addr, new_fd)){
    case 1:
      /* Ler usuario */
      sendStr(new_fd, "Digite o nome do usuario a ser buscado:\0");
      leString(their_addr, new_fd, nome);

      /* Busca nome no banco de dados */
      if(findUser(nome,pwd)){     
	/*verifica senha*/
	sendStr(new_fd, "Digite a senha do usuario:\0");
	leString(their_addr, new_fd, senha);

	if(!strcmp(senha,pwd)){
	  user=agenda_init(nome);
	  menu2(new_fd, their_addr, user);
	}
	else{
	  sendStr(new_fd, "Senha nao confere! Digite m para voltar ou q para sair:\0");
	  leString(their_addr, new_fd, again);
	  if(strcmp("q",again)==0) 
	    exit(1);
	}
      }
      else{
	sendStr(new_fd, "Usuario inexistente! Digite m para voltar ou q para sair:\0");
	leString(their_addr, new_fd, again);

	/*saida do programa*/
	if(strcmp("q",again)==0) 
	exit(1);

      break;
  
      }
      break;

    case 2:

      /* Criar um usuario */
      sendStr(new_fd, "Digite o nome do usuario a ser criado:\0");
      leString(their_addr, new_fd, nome);
      sendStr(new_fd, "Digite a senha do usuario:\0");
      leString(their_addr, new_fd, senha);

      /* Verifica se nome ja existe */
      if(newUser(nome,senha)==1){
	user=agenda_init(nome);
	menu2(new_fd, their_addr,user);
      }
      else{
	sendStr(new_fd, "Usuario já existente! Digite m para voltar ou q para sair:\0");
	leString(their_addr, new_fd, again);

	/*saida do programa */
	if(strcmp("q",again)==0)
	  close(new_fd);  // mata conexao com cliente
	  exit(1);
      }
	break;
    default:
      user_destroy(user);
      close(new_fd);  // mata conexao com cliente
      return;
      break;
    }
  }
}

void menu2(int new_fd, struct sockaddr_storage their_addr, User *user){
  char nome[20]="", dia[5]="", hora[5]="", minuto[5]="", task[1000]="", again[1]="";
  char str[1000]="";
  char menu[1000]="Escolha uma opcao:\n\
                  Opcao 1 - Marcar um compromisso\n\
                  Opcao 2 - Desmarcar um compromisso\n\
                  Opcao 3 - Obter um compromisso marcado para um horario de um dia\n\
                  Opcao 4 - Obter todos os compromissos marcados para um dia\n\
                  Opcao 5 - Obter todos os compromissos do mes\n\
                  Opcao 6 - Voltar\0";
 
  /*Recupera agenda do usuario, apos login*/
  loadCal(user);

  while(1){
    sendStr(new_fd, "Escolha uma opcao:\n\
                  Opcao 1 - Marcar um compromisso\n\
                  Opcao 2 - Desmarcar um compromisso\n\
                  Opcao 3 - Obter um compromisso marcado para um horario de um dia\n\
                  Opcao 4 - Obter todos os compromissos marcados para um dia\n\
                  Opcao 5 - Obter todos os compromissos do mes\n\
                  Opcao 6 - Voltar\0");
    switch(leOpcao(their_addr, new_fd)){
    case 1:
      /* Marcar um compromisso */
      sendStr(new_fd, "Digite o nome do compromisso:\0");
      leString(their_addr, new_fd, task);
      sendStr(new_fd, "Digite o dia do compromisso:\0");
      leString(their_addr, new_fd, dia);
      sendStr(new_fd, "Digite o hora do compromisso:\0");
      leString(their_addr, new_fd, hora);
      sendStr(new_fd, "Digite os minutos do compromisso:\0");
      leString(their_addr, new_fd, minuto);
      set_task(atoi(dia), atoi(hora), atoi(minuto), task, user);
      
      verMes(new_fd,user);

      /*Se m retorna ao menu, se q salva agenda sai*/
      leString(their_addr, new_fd,again);
      if(strcmp("q",again)==0) {
	saveCal(user);
  user_destroy(user);
	close(new_fd);  // mata conexao com cliente
	exit(1);
      }
      break;
    case 2:
      /* Desmarcar um compromisso */
      sendStr(new_fd, "Digite o nome do compromisso a ser desmarcado:\0");
      leString(their_addr, new_fd, str);
      if(delTask(user, str))
        sendStr(new_fd, "\nCompromisso desmarcado\nDigite m para voltar ao menu anterior ou q para sair\n\0");
      else
        sendStr(new_fd, "\nNao foi encontrado nenhum compromisso registrado com esse nome\nDigite m para voltar ao menu anterior ou q para sair\n\0");

      /*Se m retorna ao menu, se q salva agenda sai*/
      leString(their_addr, new_fd,again);
      if(strcmp("q",again)==0){
	saveCal(user);
  user_destroy(user);
	close(new_fd);  // mata conexao com cliente
	exit(1);
      }
      break;
    case 3:
      /* Obter compromissos de um dia em determinada hora */
      sendStr(new_fd, "Digite o dia:\0");
      leString(their_addr, new_fd, dia);
      sendStr(new_fd, "Digite as horas:\0");
      leString(their_addr, new_fd, hora);
      verHora(new_fd,user,atoi(dia),atoi(hora));
      
      /*Se m retorna ao menu, se q salva agenda sai*/
      leString(their_addr, new_fd,again);
      if(strcmp("q",again)==0){ 
	saveCal(user);
  user_destroy(user);
	close(new_fd);  // mata conexao com cliente
	exit(1);
      }
      break;
    case 4:
      /* Obter todos os compromissos marcados para um dia */
      sendStr(new_fd, "Digite o dia:\0");
      leString(their_addr, new_fd, dia);
      verDia(new_fd,user,atoi(dia));

      /*Se m retorna ao menu, se q salva agenda sai*/
      leString(their_addr, new_fd,again);
      if(strcmp("q",again)==0){ 
	saveCal(user);
  user_destroy(user);
	close(new_fd);  // mata conexao com cliente
	exit(1);
      }
      break;
    case 5:
      /* Obter todos os compromissos do mes */
      verMes(new_fd,user);

      /*Se m retorna ao menu, se q salva agenda sai*/
      leString(their_addr, new_fd,again);
      if(strcmp("q",again)==0){ 
	saveCal(user);
  user_destroy(user);
	close(new_fd);  // mata conexao com cliente
	exit(1);
      }
      break;
    default:
      saveCal(user);
      user_destroy(user);
      return;
      break;
    }
  }
}

void leString(struct sockaddr_storage their_addr, int sockfd, char string[]){

  int numbytes;
  char s[INET6_ADDRSTRLEN];
  socklen_t addr_len = sizeof their_addr;

  if ((numbytes = recvfrom(sockfd, string, MAXDATASIZE-1 , 0,
			   (struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
  }
  printf("listener: got packet from %s\n",
	 inet_ntop(their_addr.ss_family,
		   get_in_addr((struct sockaddr *)&their_addr),
		   s, sizeof s));
  printf("listener: packet is %d bytes long\n", numbytes);
  printf("listener: packet contains \"%s\"\n", string);
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

void sendStr(int new_fd, char str[]){
  if (send(new_fd, str , strlen(str) + 1, 0) == -1)
    perror("send");
}
