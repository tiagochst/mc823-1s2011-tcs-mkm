#ifndef AGENDA_H_
#define AGENDA_H_


typedef struct agenda {
  struct agenda *next;
  int dia;
  int hora, min;
  char task[256];
} Agenda;


typedef struct user {
  struct agenda *tasks;
  char user[20];
} User;


#endif /*CONJUNTO_H_*/

