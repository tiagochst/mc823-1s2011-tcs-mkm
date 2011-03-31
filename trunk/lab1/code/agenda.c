#include <stdlib.h>
#include <string.h>
#include "agenda.h"

/* INICIALIZA AGENDA
 * gera o nó cabeça e o devolve */
User * agenda_init(char nome[]) {
  User *y = (User *) malloc(sizeof(User));
  strcpy(y.user,nome);
  y->tasks = NULL;
  return y;
}

/* DESTRÓI AGENDA
 * desaloca todos os nós */
void user_destroy(User *u) {
  User *next;

  next = u->taks;
  free(u);
  for (u = next; u != NULL; u = next) {
    next = u->next;
    free(u);
  }
}

int angenda_vazia(User *a) {
  return (a->tasks == NULL)?(1):(0);
}

