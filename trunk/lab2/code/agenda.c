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

/* DESTROI AGENDA
 * desaloca todos os nos */
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

/* funcao booleana que verifica se a agenda esta vazia */
int agenda_vazia(User *a) {
  return (a->tasks == NULL)?(1):(0);
}

/* remover um compromisso pelo seu nome */ 
int delTask( User *u, char nome[]){
  int cmp;
  Agenda *a,*ant;
  Agenda *newTask;

  a=u->tasks;
  if (a ==NULL) /* Agenda vazia*/
    return 0;
  
  /* primeiro no da cabeca */
  if(cmp=strcmp(nome,a->task)==0){
    printf("sou eu!\n");
    u->tasks=a->next;
    free(a);
    return 1; 
  }
  ant=a;
  /* Percorre a lista ligada procurando pelo compromisso a ser removido */
  for (a=a->next; a != NULL; a =a->next) {
    if( strcmp(nome,a->task)==0){
      ant->next=a->next;
      free(a);
      return 1;
    }
    ant=a;
  }
  
  return 0;
}

/* Aloca um novo no com as informacoes do novo compromisso, e o insere na ordenadamente na lista ligada
 * Retorna 1: se o compromisso marcado foi inserido com sucesso
 * Retorna 0: caso contrario */
int set_task(int dia,int hora,int min,char task[], User *u){
  int cmp;
  Agenda *a,*next,*ant;
  Agenda *newTask = task_init(dia, hora,min,task);
 
  next=u->tasks;
  if (next ==NULL)
    u->tasks=newTask;
  else if(next!=NULL){ /* Agenda vazia? */
    /* Ja eh a menor? */
    if(compData(newTask,next)==1){
      printf("sou a menor!\n");
      newTask->next=u->tasks;
      u->tasks=newTask;
      return 1; 
    }
    
    /* insere ordenado usando insertion */
    for (a = next; a != NULL; a = next) {
      cmp=compData(newTask,a); /* Verifica se a data eh maior ou menor */
      if(cmp==-1){ /* Se eh maior */
	ant=a;
	next = a->next;
      }
      else if(cmp==1){ /* Se eh menor */
	newTask->next = a;
	ant->next=newTask;
	break;
      }
      else{ /* Se forem simultaneos, o compromisso nao eh inserido */
	free(newTask);
	return 0;
      }
    }
    /*ultimo compromisso*/  
    ant->next=newTask;
  }
  return 1;
}

/* 
   Retorna:
   1 se dia do novo compromisso é menor
   -1 se dia do novo compromisso é maior
   0 se dia do novo compromisso tem mesmo  horário
*/
int compData(Agenda *newTasks,Agenda *tasks){

  if(newTasks->dia < tasks->dia)
    return ANTES;
  else if(newTasks->dia > tasks->dia)
    return DEPOIS;
  else if (newTasks->hora < tasks->hora)	
    return ANTES;
  else if (newTasks->hora > tasks->hora)	
    return DEPOIS;
  else if (newTasks->min < tasks->min)	
    return ANTES;
  else if (newTasks->min > tasks->min)	
    return DEPOIS;

  return SIMULTANEO;
}

/* INICIALIZA COMPROMISSOS
 * Aloca o no compromisso com as informacoes e o devolve o seu apontador */
Agenda * task_init(int dia,int hora,int min,char task[]) {
  Agenda *newTask = (Agenda *) malloc(sizeof(Agenda));
  
  newTask->dia=dia;
  newTask->hora=hora;
  newTask->min=min;
  strcpy(newTask->task,task);
  newTask->next=NULL;

  return newTask;
}

/* Imprime todos os compromissos do mes envia para o cliente */
int verMes(int new_fd, User *u,struct sockaddr_storage their_addr){
  Agenda *next,*a;
  char mes[1000]="=== Mes de ABRIL ===\n"; 
  char comp[1000],num[5];
  
  next=u->tasks;

  /* Percorre a lista ligada e concatena cada compromisso numa string */
  for (a = next; a != NULL; a = a->next) {
    cpComp(a,comp);
    strcat(mes,comp);
    strcpy(comp,"");
  }
  printf("%s",mes);
  strcat(mes,"\nDigite m para voltar ao menu anterior ou q para sair\n");
  sendMsg(new_fd, mes,their_addr); /* Envia para o cliente toda as informacoes */

  return 0;
}

/* Dado um dia, a funcao retorna todos os compromissos daquele dia */
int verDia(int new_fd, User *u, int dia,struct sockaddr_storage their_addr){
  Agenda *next,*a;
  char mes[1000]="=== Mes de ABRIL ===\n"; 
  char comp[1000];
  
  next=u->tasks;

  /* Percorre a lista ligada em busca dos compromissos daquele dia */
  for (a = next; a != NULL; a=a->next) {
    if(a->dia==dia){
      cpComp(a,comp);
      strcat(mes,comp);
    }
    else if(a->dia>dia)/* Dias ordenados - ultrapassou data */
      break;
  }

  strcat(mes,"\nDigite m para voltar ao menu anterior ou q para sair\n");
  sendMsg(new_fd, mes,their_addr); /* Envia para o cliente os compromissos */

  return 0;
}

/* Dado um dia e uma hora, retorna para o cliente todos os compromissos correspondentes */
int verHora(int new_fd, User *u, int dia, int hora,struct sockaddr_storage their_addr){
  Agenda *next,*a;
  char mes[1000]="=== Mes de ABRIL ===\n"; 
  char comp[1000];
  
  next=u->tasks;

  /* Percorre a lista ligada procurando pelos compromissos correpondentes a hora e o dia */
  for (a = next; a != NULL; a=a->next) {
    if(a->dia==dia && a->hora==hora){
      cpComp(a,comp);
      strcat(mes,comp);
      strcat(mes,"\n");
      strcpy(comp,"");/* limpeza da variavel */
    }
    else if(a->dia>dia)/* Dias ordenados - ultrapassou data */
      break;
  }

  strcat(mes,"\nDigite m para voltar ao menu anterior ou q para sair\n");
  sendMsg(new_fd, mes,their_addr); /* Envia para o cliente */
  return 0;

}

/* Copia compromiso para visuzalizacao */
void cpComp(Agenda *a, char comp[]){
  char num[5];

  strcpy(comp,""); /* Limpeza de variaveis */
  
  strcat(comp,"\nCompromisso: ");
  strcat(comp,a->task);

  strcat(comp,"\nDia:");
  snprintf(num, sizeof(num)-1, "%d", a->dia);
  strcat(comp,num);

  strcat(comp,"\nHora:");
  snprintf(num, sizeof(num)-1, "%d", a->hora);
  strcat(comp,num);

  strcat(comp,"\nMin:");
  snprintf(num, sizeof(num)-1, "%d", a->min);
  strcat(comp,num);

  return;
}
