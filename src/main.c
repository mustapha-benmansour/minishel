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
#include "builtin.h"





int main(int argc, char *argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char *cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t *current;
  while (1) {
    // Effacer les contenus de cmdline, cmdtoks et cmds
    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    cmdline[0]='\0';
    for(int i=0;i<MAX_CMD_SIZE;i++){
      init_cmd(&cmds[i]);
      cmdtoks[i]=NULL;
    }
    // Afficher un prompt
    printf("$ ");

    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin) == NULL)
      break;
    cmdline[strlen(cmdline) - 1] = '\0';

    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    trim(cmdline);
    if (cmdline[0]=='\0') 
      continue;
    //   - ajouter d'éventuels espaces autour de ; ! || && & ...
    // 4
    separate_s(cmdline,"2>&1",MAX_LINE_SIZE);
    separate_s(cmdline,"2>-1",MAX_LINE_SIZE);
    // 2
    separate_s(cmdline,"||",MAX_LINE_SIZE);
    separate_s(cmdline,"&&",MAX_LINE_SIZE);
    separate_s(cmdline,">>",MAX_LINE_SIZE);
    separate_s(cmdline,"<<",MAX_LINE_SIZE);
    // 1
    separate_s(cmdline,";",MAX_LINE_SIZE);
    separate_s(cmdline,"|",MAX_LINE_SIZE);
    separate_s(cmdline,"&",MAX_LINE_SIZE);
    separate_s(cmdline,">",MAX_LINE_SIZE);
    separate_s(cmdline,"<",MAX_LINE_SIZE);
    //   - supprimer les doublons d'espaces
    clean(cmdline);
    //   - traiter les variables d'environnement
    substenv(cmdline,MAX_LINE_SIZE);
    // Découper la ligne dans cmdtoks
    strcut(cmdline,' ',cmdtoks,MAX_CMD_SIZE);
    // Traduire la ligne en structures cmd_t dans cmds
    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue
    if (parse_cmd(cmdtoks,cmds,MAX_CMD_SIZE))
      continue;

    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
    for (current = cmds; current != NULL;) {
      int err;
      if (is_builtin(current->path)){
        err=builtin(current);
      }else{
        err=exec_cmd(current);
      }
      if (err) current->status=err;
      if (current->status){
        if (!current->next_failure) break; // exit with error
        current=current->next_failure;
      }else{
        if (current->next_success){
          current=current->next_success;
        }else if (current->next){
          current=current->next;
        }else{
          current=NULL;
        }
      }
    }
  }

  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
