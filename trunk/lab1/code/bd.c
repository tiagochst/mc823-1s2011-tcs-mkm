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

  strcat(nome, "\n"); /*Formatacao para comaparacao*/

  /* Formato aqruivo: usuario\nsenha\n */
  pFile = fopen("users.txt", "r"); /*arquivo com nome de usuarios*/

  if (pFile == NULL) perror("Error opening file");
  else
  {

    /*Le 100 caracteres ou até o final da linha*/
    while (fgets(user, 100, pFile) != NULL)
    {
      fgets(pwd, 100, pFile); /*senha do usuario*/
      if (strcmp(user, nome) == 0)
      {
        ;
        fclose(pFile);
        strncpy(arq, nome, strlen(nome) - 1);
        /* Cria o arquivo do usuario, caso aquele nao exista */
        pFile = fopen(arq, "a");
        fclose(pFile);
        return 1;
      }
    }
  }
  fclose(pFile);
  return 0;
}

/* Insere USUARIO
 * Retorna 1: se usuario foi inserido
 * Retorna 0: caso contrario*/
int newUser(char nome[], char senha[])
{
  FILE * pFile;
  char pwd[20], arq[20] = "";

  if (findUser(nome, pwd) == 0)
  {
    pFile = fopen("users.txt", "a"); /*arquivo com nome de usuarios*/
    if (pFile == NULL)
      perror("Error opening file");
    else
    {
      fseek(pFile, 0, SEEK_END);
      //strcat(senha,"\n"); /*Formatacao para comaparacao*/
      fputs(nome, pFile);
      fputs(senha, pFile);
      fputs("\n", pFile);
      fclose(pFile);
      /* Cria a agenda para o usuario */

      strncpy(arq, nome, strlen(nome) - 1);
      pFile = fopen(arq, "w");
      fclose(pFile);
      return 1;
    }
  }

  fclose(pFile);
  return 0;
}

/*Le toda a agenda do usuario*/
int loadCal(User *user)
{
  FILE * pFile;
  char nome[20];
  char dia[5], hora[5], min[5], task[100], arq[100];
  Agenda *atual;
  int i = 0; /*numero de compromissos*/

  /*Abre agenda do usuario*/
  strncpy(nome, user->name, strlen(user->name) - 1);
  strcat(nome, "\0");
  pFile = fopen(nome, "r"); /*arquivo com nome de usuarios*/
  if (pFile == NULL)
    return i;

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

/* Insere Compromissos na agenda
 * Retorna 1: se compromissos inseridos
 * Retorna 0: caso contrario*/
int saveCal(User *user)
{
  FILE * pFile;
  char pwd[20], arq[20] = "", nome[20];
  Agenda *atual;

  strncpy(nome, user->name, strlen(user->name) - 1);
  strcat(nome, "\0");
  pFile = fopen(nome, "w"); /*arquivo com nome de usuarios*/
  if (pFile == NULL)
    return 0;
  else
  {
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
