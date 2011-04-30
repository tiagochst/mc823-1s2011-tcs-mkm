 /*
** server.c -- a stream socket server demo
*/
#include "agenda.h"
#include <sys/time.h>

/* Estrutura para analise de tempo em microsegundos */
struct timeval  first, second, lapsed;
struct timezone tzp; 

void serverTimeRecv(struct timeval first,struct timeval second){

  double t2=first.tv_sec+(first.tv_usec/1000000.0); 
  double t3=second.tv_sec+(second.tv_usec/1000000.0); 

  FILE * pFile;
  pFile = fopen("serverTime.dat", "a"); /*arquivo com tempos do servidor*/

  if (pFile == NULL) 
    return ;
  
  /* if (first.tv_usec > second.tv_usec) { 
    second.tv_usec += 1000000; 
    second.tv_sec--; 
  } */
 
  fseek(pFile, 0, SEEK_END);
  fprintf(pFile,"%f \n" ,t3-t2);
  fclose(pFile);

  return;
}


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
  int sockfd,new_fd;
 struct addrinfo hints, *servinfo, *p;
 int rv;
 int numbytes;
 struct sockaddr_storage their_addr;
 char buf[MAXDATASIZE];
 socklen_t addr_len;
 socklen_t sin_size;
 struct sigaction sa;
 int yes=1;
 char s[INET6_ADDRSTRLEN],tempo[5],str[5];


 memset(&hints, 0, sizeof hints);
 hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
 hints.ai_socktype = SOCK_DGRAM;
 hints.ai_flags = AI_PASSIVE; // use my IP

 if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
   return 1;
 }
 
 // loop through all the results and bind to the first we can
 for(p = servinfo; p != NULL; p = p->ai_next) {
   if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
     perror("listener: socket");
     continue;
   }
   
   if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
     close(sockfd);
     perror("listener: bind");
     continue;
   }
   break;
 }
 
 if (p == NULL) {
   fprintf(stderr, "listener: failed to bind socket\n");
   return 2;
 }
 
 printf("server: waiting for connections...\n");
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
 
 printf("\n%d",sockfd); 
 menu(sockfd, their_addr);

 freeaddrinfo(servinfo);
 return 0;
}


void menu(int new_fd, struct sockaddr_storage their_addr){
  User *user;
  char nome[20],senha[20],pwd[20],again[1];
  char str[1000];


  while(1){
    printf("Vou mandar o menu");
    printf("FD = %d",new_fd);

    sendStr(new_fd,"Escolha uma opcao:\n\
                  Opcao 1 - Entrar como um usuario\n\
                  Opcao 2 - Criar um usuario\n\
                  Opcao q - Sair\n\0",their_addr);

    switch(leOpcao(their_addr, new_fd)){
    case 1:
      /* Ler usuario */
      sendStr(new_fd, "Digite o nome do usuario a ser buscado:\0",their_addr);
      leString(their_addr, new_fd, nome);

      /* Busca nome no banco de dados */
      if(findUser(nome,pwd)){     
	/*verifica senha*/
	sendStr(new_fd, "Digite a senha do usuario:\0",their_addr);
	leString(their_addr, new_fd, senha);

	if(!strcmp(senha,pwd)){
	  user=agenda_init(nome);
	  menu2(new_fd, their_addr, user);
	}
	else{
	  sendStr(new_fd, "Senha nao confere! Digite m para voltar ou q para sair:\0",their_addr);
	  leString(their_addr, new_fd, again);
	  if(strcmp("q",again)==0) 
	    exit(1);
	}
      }
      else{
	sendStr(new_fd, "Usuario inexistente! Digite m para voltar ou q para sair:\0",their_addr);
	leString(their_addr, new_fd, again);

	/*saida do programa*/
	if(strcmp("q",again)==0) 
	exit(1);

      break;
  
      }
      break;

    case 2:

      /* Criar um usuario */
      sendStr(new_fd, "Digite o nome do usuario a ser criado:\0",their_addr);
      leString(their_addr, new_fd, nome);
      sendStr(new_fd, "Digite a senha do usuario:\0",their_addr);
      leString(their_addr, new_fd, senha);

      /* Verifica se nome ja existe */
      if(newUser(nome,senha)==1){
	user=agenda_init(nome);
	menu2(new_fd, their_addr,user);
      }
      else{
	sendStr(new_fd, "Usuario já existente! Digite m para voltar ou q para sair:\0",their_addr);
	leString(their_addr, new_fd, again);

	/*saida do programa */
	if(strcmp("q",again)==0)
	  close(new_fd);  // mata conexao com cliente
	  exit(1);
      }
	break;
    default:
      return;
      break;
    }
  }
  return;

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
                  Opcao 6 - Voltar\0",their_addr);
    switch(leOpcao(their_addr, new_fd)){
    case 1:
      /* Marcar um compromisso */
      sendStr(new_fd, "Digite o nome do compromisso:\0",their_addr);
      leString(their_addr, new_fd, task);
      sendStr(new_fd, "Digite o dia do compromisso:\0",their_addr);
      leString(their_addr, new_fd, dia);
      sendStr(new_fd, "Digite o hora do compromisso:\0",their_addr);
      leString(their_addr, new_fd, hora);
      sendStr(new_fd, "Digite os minutos do compromisso:\0",their_addr);
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
      sendStr(new_fd, "Digite o nome do compromisso a ser desmarcado:\0",their_addr);
      leString(their_addr, new_fd, str);
      if(delTask(user, str))
        sendStr(new_fd, "\nCompromisso desmarcado\nDigite m para voltar ao menu anterior ou q para sair\n\0",their_addr);
      else
        sendStr(new_fd, "\nNao foi encontrado nenhum compromisso registrado com esse nome\nDigite m para voltar ao menu anterior ou q para sair\n\0",their_addr);

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
      sendStr(new_fd, "Digite o dia:\0",their_addr);
      leString(their_addr, new_fd, dia);
      sendStr(new_fd, "Digite as horas:\0",their_addr);
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
      sendStr(new_fd, "Digite o dia:\0",their_addr);
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
  int rcv=0;    
  
  while(rcv< 148){
    
    if ((numbytes = recvfrom(sockfd, string, MAXDATASIZE-1 , 0,
			     (struct sockaddr *)&their_addr, &addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }
    rcv+=numbytes;
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

  int rcv=0;    
  
  while(rcv< 148){

  if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0,
			   (struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
  }
    rcv+=numbytes;
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


/*Vamos alterar da função send para sendto por
  causa do protocolo UDP*/
void sendStr(int fd, char str[],struct sockaddr_storage their_addr){
   size_t nleft;
   ssize_t nsendto;
   nleft = strlen(str);

   socklen_t addr_len;
   addr_len = sizeof(their_addr);
   printf("%s",fd);
   while(nleft > 0){
     nsendto = sendto(fd, str , nleft + 1, 0, 
		      (struct sockaddr *)&their_addr, addr_len);
     if (nsendto ==-1){
       printf("\nEnviei %d\n",nsendto);
       printf("\nError::sendStr\n");
       return;	/* error */
     }
     else{
     //printf("\nEnviei %d\n",nsendto);
     nleft -= nsendto;
     printf("\nFalta %d\n",nleft);
     }
   }
   printf("SAINDO");
   return;  
}


