#ifndef AGENDA_H_
#define AGENDA_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to
#define MAXDATASIZE 1000
#define BACKLOG 10     // how many pending connections queue will hold

typedef struct agenda {
  struct agenda *next;
  int dia;
  int hora, min;
  char task[256];
} Agenda;


typedef struct user {
  struct agenda *tasks;
  char name[20];
} User;

/* Funcoes */
void menu(int new_fd, struct sockaddr_storage their_addr);
void menu2(int new_fd, struct sockaddr_storage their_addr);
void sendStr(int sockfd, char str[]);
int leOpcao(struct sockaddr_storage their_addr, int sockfd);
void leString(struct sockaddr_storage their_addr, int sockfd , char string[]);
User * agenda_init(char nome[]);
void user_destroy(User *u);
int angenda_vazia(User *a);

#endif /*CONJUNTO_H_*/

