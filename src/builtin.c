/*
  Projet minishell - Licence 3 Info - PSI 2024
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des commandes internes du minishell (implémentation).
 
 */

#include "cmd.h"
#include <string.h>

int is_builtin(const char* cmd) {
  //return strcmp(cmd,"cd")==0;
}

int builtin(cmd_t* cmd) {

}

int cd(const char* path, int fderr) {

}

int export(const char* var, const char* value, int fderr) {

}

int exit_shell(int ret, int fderr) {

}
