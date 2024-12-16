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
  size_t len=strlen(str);
  char quot=0;
  char quot_prev=0;
  char new=1;
  for (size_t i = 0 ; i < len ;i++){
    if ( (str[i]=='"' || str[i]=='\'')  &&  (!quot || quot==str[i]) ){
      quot_prev=quot?quot:0;
      quot=quot?0:str[i];
    }else if (new){
      if (count >= max) return count;
      tokens[count++]=&str[i];
      new=0;
    }else if (!quot && (str[i]==sep || i==len-1)){
      if ( i>0 && str[i-1]==quot_prev)
        str[i-1]='\0';
      else
        str[i]='\0';
      quot_prev=0;
      new=1;
    }
  }
	return count;
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

}



