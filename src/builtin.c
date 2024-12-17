/*
		Projet minishell - Licence 3 Info - PSI 2024

		Nom :
		Prénom :
		Num. étudiant :
		Groupe de projet :
		Date :

		Gestion des commandes internes du minishell (implémentation).

 */

#include "builtin.h"
#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *functions[] = {"cd", "exit", "export", "unset",
						   NULL}; // Tableau des commandes internes

int is_builtin(const char *cmd) {
	for (int i = 0; functions[i] != NULL;) {
		if (strcmp(functions[i], cmd) == 0)
			return 1;
  	}
  	return 0;
}

int builtin(cmd_t *cmd) {
  	if (strcmp(cmd->argv[0], "cd") == 0) {
		return cmd->argv[1] ? cd(cmd->argv[1], cmd->stderr)
						: cd(getenv("HOME"), cmd->stderr);
  	}
  	if (strcmp(cmd->argv[0], "export") == 0) {
		return export(cmd->argv[1], cmd->argv[2], cmd->stderr);
  	}
  	if (strcmp(cmd->argv[0], "exit") == 0) {
		return exit_shell(cmd->argv[1] ? (atoi(cmd->argv[1])) : 0,
					 cmd->stderr); // Exit avec code
  	}
  	return 0;
}

int cd(const char *path, int fderr) {
	if (chdir(path) == -1) {
		write(fderr, "cannot change dir", 17);
		return -1;
	} else {
		return 0;
	}
}

int export(const char *var, const char *value, int fderr) {
	if (!var || setenv(var, value, 1) == -1) {
		write(fderr, "cannot export var", 17);
		return -1;
	} else {
		return 0;
	}
}

int exit_shell(int ret, int fderr) {
	char e[64];
	int len = sprintf(e, "exit code %d\n", ret);
	write(fderr, e, len);
	exit(ret);
}
