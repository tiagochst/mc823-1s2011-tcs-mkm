#include <stdlib.h>
#include <string.h>
#include "agenda.h"

/* INICIALIZA AGENDA
 * gera o nó cabeça e o devolve */
User * agenda_init(char nome[]) {
  User *y = (User *) malloc(sizeof(User));
  strcpy(y->name,nome);
  y->tasks = NULL;
  return y;
}

/* DESTRÓI AGENDA
 * desaloca todos os nós */
void user_destroy(User *u) {
  Agenda *next;
  Agenda *a;

  next = u->tasks;
  free(u);
  for (a = next; a != NULL; a = next) {
    next = a->next;
    free(a);
  }
}

int agenda_vazia(User *a) {
  return (a->tasks == NULL)?(1):(0);
}

int set_task(int dia,int hora,char task[], User u){
  int cmp;
  Agenda *a,*next,*ant;
  Agenda *newTask = (Agenda *) malloc(sizeof(Agenda));
  newTask.dia=dia;
  newTask.hora=hora;
  strcpy(newTask.tasks,task);
  newTask.next=Null;

  next=u->tasks;
  /*Já é a menor?*/
  if(compData(newTasks,next)==1){
    newTask->next=u->tasks->next;
    u->tasks=newTask;
    return 1; 
  }

  /* insere ordenado usando insertion sort */
  for (a = next; a != NULL; a = next) {
    cmp=compData(newTasks,a); 
    if(cmp==-1){
      ant=a;
      next = a->next;
    }
    else if(cmp==1){
      newTask.next = a;
      ant->next=newTask;
    }      
    else{
      return 0;
    }
  }
    /*ultimo compromisso*/  
  ant=newTask;
  return 1;
}

/* 
   Retorna:
   1 se dia do novo compromisso é menor
   -1 se dia do novo compromisso é maior
   0 se dia do novo compromisso tem mesmo  horário
*/
int compData(Agenda *newTasks,Agenda *tasks){

  if(newTasks.dia<tasks.dia)
    return 1;
  else if(newTasks.dia>tasks.dia)
    return -1;
  else if (newTasks.hora<tasks.hora)	
    return 1;
  else if (newTasks.hora>tasks.hora)	
    return -1;
  else if (newTasks.min<tasks.min)	
    return 1;
  else if (newTasks.min>tasks.min)	
    return 1;

  return 0;
}
