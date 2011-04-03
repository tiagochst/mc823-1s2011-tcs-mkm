#include <stdio.h>
#include <string.h>
#include "bd.h"


/* Verifica USUARIOS
 * compara no arquivo fp uma lista de usuarios
 * se uxuario existe retorna 1, senao 0 */
int findUser(char nome[],char pwd[]) {
  char user [30];
  FILE * pFile;

  strcat(nome,"\n"); /*Formatacao para comaparacao*/

  /* Formato aqruivo: usuario\nsenha\n */
  pFile = fopen ("users.txt" , "r");/*arquivo com nome de usuarios*/

  if (pFile == NULL) perror ("Error opening file");
  else {

    /*Le 100 caracteres ou até o final da linha*/
    while(fgets (user, 100, pFile)!= NULL){
      fgets (pwd, 100, pFile);/*senha do usuario*/
      if(strcmp(user,nome)==0){;
	fclose (pFile);
	return 1;
      }
    }
  }
  
  fclose (pFile);
  return 0;
}
/* Insere USUARIO
 * Retorna 1: se usuario foi inserido
 * Retorna 0: caso contrario*/
int newUser(char nome[],char senha[]){
  FILE * pFile;
  char pwd[20];

  if(findUser(nome,pwd)==0){
    pFile = fopen ("users.txt" , "a");/*arquivo com nome de usuarios*/
    if (pFile == NULL) 
      perror ("Error opening file");
    else {
      fseek ( pFile ,0 , SEEK_END );
      //strcat(nome,"\n"); /*Formatacao para comaparacao*/
      fputs ( nome , pFile );
      fputs ( senha , pFile );
      fclose (pFile);
      return 1;
    }
  }

  fclose (pFile);
  return 0;
}
