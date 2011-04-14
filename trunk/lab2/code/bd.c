#include <stdio.h>
#include <string.h>
#include "agenda.h"

/* Verifica USUARIOS
 * compara no arquivo fp uma lista de usuarios
 * se uxuario existe retorna 1, senao 0 */
int findUser(char nome[], char pwd[])
{
  char user [30], arq[20] = "";
  FILE * pFile;

  /* Formato aqruivo: usuario\nsenha\n */
  pFile = fopen("users.txt", "r"); /*arquivo com nome de usuarios*/

  if (pFile == NULL) {
    printf("\nFIND USER NULL FILE");
    return 0;
  }
  else {

    /*Le 100 caracteres ou ate o final da linha*/
    while (fscanf(pFile, "%[^\n]", user) != EOF)
    {
      fgetc(pFile);
      fscanf(pFile, "%[^\n]", pwd); /* senha do usuario, nao eh usado, somente para leitura do arquivo */
      fgetc(pFile);
      if (strcmp(user, nome) == 0)  /* Verifica se o eh o usuario buscado */
      {
        fclose(pFile);
        /* Cria o arquivo do usuario, caso aquele nao exista */
        pFile = fopen(nome, "a");
        fclose(pFile);
        return 1; /* Devolve 1 se o usuario buscado foi encontrado no arquivo users.txt */
      }
    }
  }
  fclose(pFile);
  return 0; /* Devolve 0 caso o usuario buscado nao esteja cadastrado */
}

/* Insere USUARIO
 * Retorna 1: se usuario foi inserido
 * Retorna 0: caso contrario*/
int newUser(char nome[], char senha[])
{
  FILE * pFile;
  char pwd[20], arq[20] = "";

  if (findUser(nome, pwd) == 0)  /* Verifica se o usuario que se deseja cadastrar ja existe */
  {
    pFile = fopen("users.txt", "a"); /*arquivo com nome de usuarios*/
    if (pFile == NULL)
      perror("Error opening file");
    else
    {
      fseek(pFile, 0, SEEK_END); /* O novo usuario eh colocado no final do arquivo */
      fputs(nome, pFile); /* Nome */
      fputs("\n", pFile);
      fputs(senha, pFile); /* Senha */
      fputs("\n", pFile);
      fclose(pFile);

      /* Cria a agenda para o usuario */
      pFile = fopen(nome, "w");
      fclose(pFile);

      return 1;
    }
  }

  fclose(pFile);
  return 0;
}

/*Le toda a agenda do usuario em arquivo e passa para memoria*/
int loadCal(User *user)
{
  FILE * pFile;
  char nome[20]="";
  char dia[5], hora[5], min[5], task[100], arq[100]="";
  Agenda *atual;
  int i = 0; /*numero de compromissos*/

  /*Abre agenda do usuario*/
  strcpy(nome, user->name);
  pFile = fopen(nome, "r"); /*arquivo com nome de usuarios*/
  if (pFile == NULL){
    printf("\nnome: %s --- %s",user->name,user->name[strlen(user->name) - 1]);
    printf("\nnome: %s",nome);
    printf("\nERROR -- LOAD CAL NULL FILE\n");
    return i;
  }
  else
  {
    /*Primeiro evento*/
    if (fscanf(pFile, "%[^\n]", task) != EOF)
    {/*evento*/
      fgetc(pFile);
      fscanf(pFile, "%[^\n]", dia); /*dia*/
      fgetc(pFile);
      fscanf(pFile, "%[^\n]", hora); /*hora*/
      fgetc(pFile);
      fscanf(pFile, "%[^\n]", min); /*minuto*/
      fgetc(pFile);
      printf("\nInserindo:%s %s %s %s", task, dia, hora, min);
      user->tasks = task_init(atoi(dia), atoi(hora), atoi(min), task);
      atual = user->tasks;
      i++;

      /* Percorre o arquivo lendo os compromissos */
      while (fscanf(pFile, "%[^\n]", task) != EOF)
      {
        fgetc(pFile);
        fscanf(pFile, "%[^\n]", dia); /*dia*/
        fgetc(pFile);
        fscanf(pFile, "%[^\n]", hora); /*hora*/
        fgetc(pFile);
        fscanf(pFile, "%[^\n]", min); /*minuto*/
        fgetc(pFile);
        printf("\n\narq %s", arq);
        printf("  task %s\n\n", task);
        /* Cria um novo no na lista ligada com as informacoes do compromisso */
        atual->next = task_init(atoi(dia), atoi(hora), atoi(min), task);
        atual = atual->next;
        i++;
        strcpy(arq, "");
        strcpy(task, "");

      }
    }
    fclose(pFile);
    return i;
  }


  fclose(pFile);


  return i;
}

/* Insere Compromissos na agenda, passando da memoria para arquivo
 * Retorna 1: se compromissos inseridos
 * Retorna 0: caso contrario*/
int saveCal(User *user)
{
  FILE * pFile;
  char pwd[20], arq[20] = "", nome[20];
  Agenda *atual;

  strcpy(nome, user->name);
  pFile = fopen(nome, "w"); /*arquivo com nome de usuarios*/
  if (pFile == NULL){
    printf("\nNULL - SaveCal\n");
    return 0;
  }
  else
  {
    /* Percorre a lista ligada e imprime as infomacoes de cada no no arquivo */
    for (atual = user->tasks; atual != NULL; atual = atual->next)
    {
      fputs(atual->task, pFile);
      fprintf(pFile, "\n%d\n%d\n%d\n", atual->dia, atual->hora, atual->min);
    }
    fclose(pFile);
    return 1;
  }

  fclose(pFile);
  return 0;
}
