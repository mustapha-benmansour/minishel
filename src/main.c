/*
  Projet minishell - Licence 3 Info - PSI 2024

  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :

  Interface du minishell.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd.h"
#include "parser.h"



int substenv(char* str, size_t max) {
  size_t len = strlen(str);
  size_t i=0,j,k,m;
  ssize_t add_len;

  /// $PATH  ${PATH}   
  // 0 1 2 3 4
  while(i<len-1){
    while (str[i]!='$' && str[i+1]!='\0') i++;
    char brack=str[i+1]=='{';
    char * env_var;
    char c ;
    if (brack){
      j=i+1;
      while (str[j]!='}')j++;
      str[j]='\0';
      env_var=getenv(&str[i+2]);
      str[j]='}';
      if (env_var){
        add_len=strlen(env_var)-(j-i);
        if (add_len>0){
          if (add_len+len>max) return 0;
          for (k=0;k<add_len;k++){
            for (m=add_len;m>0;m--){
              str[len+m]=str[len+m-1];
            }
          }
        }else{
          /*for (k=0;k<abs((int)add_len);k++){
            for (m=j+1;j+k;m--){
              str[len+m]=str[len+m-1];
            }
          }*/
          printf("NOT IMP");
        }
        strncpy(&str[i], env_var, strlen(env_var));
        i+=strlen(env_var);
      }else{
        i=j+1;
      }
    }else{
      j=i;
      while (str[j]!=' ' && str[j]!='\0')j++;
      c=str[j];
      str[j]='\0';
      env_var=getenv(&str[i+1]);
      str[j]=c;
      if (env_var){
        add_len=strlen(env_var)-(j-i);
        printf("add len = %d \n",add_len);
        if (add_len>0){
          if (add_len+len>max) return 0;
          for (k=0;k<add_len;k++){
            for (m=add_len;m>0;m--){
              str[len+m]=str[len+m-1];
            }
          }
        }else{
          /*for (k=0;k<abs((int)add_len);k++){
            for (m=j+1;j+k;m--){
              str[len+m]=str[len+m-1];
            }
          }*/
          //printf("NOT IMP\n");
        }
        strncpy(&str[i], env_var, strlen(env_var));
        i+=strlen(env_var);
      }else{
        i=j+1;
      }
    }
  }
  return 0;
}

int main() {
  char line[512];

  strcpy(line, "ana 'najwa P' \"A stof\"");
	printf("BEFORE => '%s'\n", line);


  /*if (1){
    substenv(line, 512);
    printf("AFTERE => '%s'\n", line);
    return 0;
  }

  if (1){
    separate_s(line, ",", 512);
    printf("AFTERE => '%s'\n", line);
    return 0;
  }*/

  char *toks[10];

  int len=strcut_s(line, ' ', toks, 10);

  for (int i=0;i<len;i++){
	printf("TOK -> (%s)\n",toks[i]);
  }

  //strcpy(line, "   na  j          wa  ");
  // if (fgets(line, 512, stdin)==NULL) return 0;
  /*printf("BEFORE => '%s'\n", line);
  trim(line);
  clean(line);
  printf("AFTERE => '%s'\n", line);*/
  return 0;
}

int __main(int argc, char *argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char *cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t *current;

  while (1) {
    // Effacer les contenus de cmdline, cmdtoks et cmds
    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)

    // Afficher un prompt
    printf("$ ");

    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin) == NULL)
      break;
    cmdline[strlen(cmdline) - 1] = '\0';

    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    //   - ajouter d'éventuels espaces autour de ; ! || && & ...
    //   - supprimer les doublons d'espaces
    //   - traiter les variables d'environnement

    // Découper la ligne dans cmdtoks

    // Traduire la ligne en structures cmd_t dans cmds
    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue

    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
    for (current = cmds; current != NULL;) {

      // Lancer la commande
      // exec_cmd(current);
    }
  }

  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
