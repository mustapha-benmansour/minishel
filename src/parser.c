/*
  Projet minishell - Licence 3 Info - PSI 2024
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <stddef.h>
#include <string.h>


int is_space(char c){
  return c==' ' || c=='\t';
}

int trim(char* str) {
  char *c;
  int len=strlen(str);
  char * last_char=&str[len-1];
  while(str!=last_char && is_space(*str)){
    c=last_char-1;
    do{
      *c=c[1];
    }while(str<c);
    *last_char='\0';
    last_char--;
  }
  while (last_char!=str && is_space(*last_char)){
    *last_char='\0';
    last_char--;
  }
  return 0;
}

int clean(char* str) {

}

int separate_s(char* str, char* s, size_t max) {

}

int substenv(char* str, size_t max) {

}

int strcut(char* str, char sep, char** tokens, size_t max) {

}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {

}
