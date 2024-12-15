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

int is_space(char c) { return c == ' ' || c == '\t'; }
int trim(char *str) {
  char *current;
  char *last_char = &str[strlen(str) - 1];
  // remove spaces from the end
  while (last_char >= str && is_space(*last_char)) {
    *last_char = '\0';
    last_char--;
  }

  // move string to the begining while string start with space
  while (str <= last_char && is_space(*str)) {
    current = str;
    while(current < last_char) {
      *current = current[1];
      current++;
    };
    *last_char = '\0';
    last_char--;
  }
  return 0;
}

int clean(char *str) {
  int was_space = 0;
  char *last_char = &str[strlen(str) - 1];
  char *current = str;
  char * picked ;
  while (current<=last_char) {
    if (is_space(*current)) {
      if (was_space) {
		picked = current;
		while (picked < last_char){
			*picked = picked[1];
			picked++;
		} 
		*last_char = '\0';
		last_char--;
      }else{
		was_space=1;
		current++;
	  } 
    }else{
		current++;
		was_space=0;
	}
  }
  return 0;
}


int strcut(char* str, char sep, char **tokens, size_t max) {
	size_t count=0;
	char * current=str;
	char * last_char = &str[strlen(str)-1];
	while (current<=last_char && count<=max){
		if (!count){
			tokens[count++]=current;
		}else if (*current==sep) {
			*current='\0';
			tokens[count++]=++current;
		}
		current++; 
	}
	return count;
}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {
	size_t count=0;
	char * current=str;
	char * last_char = &str[strlen(str)-1];
  char quot=0;
	while (current<=last_char && count<=max){
		if (!count){
			tokens[count++]=current;
		}else if ( (*current=='"' || *current=='\'')  &&  (!quot || quot==*current) ){
      quot=quot?0:*current;
    }else if (!quot && *current==sep) {
			*current='\0';
			tokens[count++]=++current;
		}
		current++; 
	}
	return count;
}

int strcut_s_aziz(char *str, char sep, char **tokens, size_t max)
{
    size_t token_cmp = 0; // Compteur de tokens
    size_t begin = 0;     // Index du token
    int inQuotes = 0;     // Flag pour savoir si on est dans des quotes
    tokens[0] = '\0'; // Initialisation du tableau tokens

    // Pour chaque caractère de la chaine
    for (size_t i = 0; token_cmp < max; i++)
    {
        // Si on a trouvé un séparateur
        if (str[i] == '\"' || str[i] == '\'')
        {
            // On est dans des quotes (" ou ')
            inQuotes = !inQuotes;
        }
        else if (!inQuotes && str[i] == sep)
        {
            // On est toujours pas dans des quotes et on a trouvé un séparateur
            tokens[token_cmp++] = str + begin; // Ajouter le token au tableau
            str[i] = '\0';                     // Remplacer le séparateur par un caractère vide
            begin = i + 1;                    
        }
    }

    tokens[token_cmp++] = str + begin; // Ajouter le token au tableau
    tokens[token_cmp] = NULL; // Terminer le tableau de tokens

    return token_cmp;
}


int separate_s(char* str, char* s, size_t max) {
  size_t len=strlen(str);
  int i,k;
  for (k=0;k<len && len+2<=max;){
    if (strchr(s, str[k])!=NULL){
      for (i=len+1;i>=k+1;i--) str[i]=str[i-2];len+=2;
      str[k+2]=' ';
      str[k+1]=str[k];
      str[k]=' ';
      k+=3;
      str[len]='\0';
    }else
      k++;
  }
  return 0;
}

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

  strcpy(line, "ana 'najwa P' stof");
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
