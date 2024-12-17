/*
	Projet minishell - Licence 3 Info - PSI 2024

	Nom :
	Prénom :
	Num. étudiant :
	Groupe de projet :
	Date :

	Gestion des processus (implémentation).

 */

#include "cmd.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int init_cmd(cmd_t *p) {

	p->wait = 1; // blocking
	p->pid = 0;
	p->status = 0; // exit code

	p->stdin = STDIN_FILENO;
	p->stdout = STDOUT_FILENO;
	p->stderr = STDERR_FILENO;

	p->path = NULL;
	for (int i = 0; i < MAX_CMD_SIZE; ++i) {
		p->argv[i] = NULL;
		p->fdclose[i] = -1;
	}
	p->next = p->next_success = p->next_failure = NULL;

	return 0;
}

#define RS_ERR -1
#define RS_OK 0
#define RS_PRC 1
#define RS_TOK 2
#define RS_ARG 3

static void set_fdclose(cmd_t *cmd, int fd) {
	int i = 0;
	while (cmd->fdclose[i] != -1)
		i++;
	cmd->fdclose[i] = fd;
}

static int resolve_stdfd(cmd_t *cmd, char *next_tk, int mode, int *outptr) {
	int fd = open(next_tk, mode, 0644);
	if (fd != -1) {
		set_fdclose(cmd, *outptr);
		*outptr = fd;
		return RS_TOK;
	}
	return RS_ERR;
}

static int resolve_token(char *tk, char *next_tk, cmd_t *cmd, cmd_t *next_cmd) {

	size_t sz = strlen(tk);
	if (sz == 1) {
		if (*tk == ';') {
			cmd->next = next_cmd;
			return RS_PRC;
		}
		if (*tk == '|') {
			int fd[2];
			if (next_cmd && pipe(fd) != -1) {
				next_cmd->stdin = fd[0];
				set_fdclose(next_cmd, fd[1]);
				cmd->stdout = fd[1];
				set_fdclose(cmd, fd[0]);
				cmd->next = next_cmd;
				return RS_PRC;
			}
			return RS_ERR;
		}
		if (*tk == '&') {
			cmd->wait = 0;
			cmd->next = next_cmd;
			return RS_PRC;
		}
		if (*tk == '>')
			return resolve_stdfd(cmd, next_tk, O_WRONLY | O_CREAT | O_TRUNC,
													 &cmd->stdout);
		if (*tk == '<')
			return resolve_stdfd(cmd, next_tk, O_RDONLY, &cmd->stdin);
	} else if (sz == 2) {
		if (*tk == '|' && tk[1] == '|') {
			cmd->next_failure = next_cmd;
			return RS_PRC;
		}
		if (*tk == '&' && tk[1] == '&') {
			cmd->next_success = next_cmd;
			return RS_PRC;
		}
		if (*tk == '2' && tk[1] == '>')
			return resolve_stdfd(cmd, next_tk, O_WRONLY | O_CREAT | O_TRUNC,
													 &cmd->stderr);
		if (*tk == '>' && tk[1] == '>')
			return resolve_stdfd(cmd, next_tk, O_WRONLY | O_CREAT | O_APPEND,
													 &cmd->stdout);
		if (*tk == '<' && tk[1] == '<')
			return resolve_stdfd(cmd, next_tk, O_RDONLY, &cmd->stdin);

	} else if (sz == 4 && *tk == '2' && tk[1] == '>' &&
						 ((tk[2] == '&' && tk[3] == '1') ||
							(tk[2] == '1' && tk[3] == '-'))) {
		if (tk[2] == '&') {
			set_fdclose(cmd, cmd->stdout);
			cmd->stderr = cmd->stdout;
		} else
			close(cmd->stderr);
		return RS_OK;
	}
	return RS_ARG;
}

int parse_cmd(char *tokens[], cmd_t *cmds, size_t max) {
	int tok_idx = -1, prc_idx = 0, arg_idx = 0;
	int result;
	while (tokens[++tok_idx]) {
		result = resolve_token(tokens[tok_idx], tokens[tok_idx + 1], &cmds[prc_idx],
													 &cmds[prc_idx + 1]);
		if (result == RS_ERR)
			return -1;
		if (result == RS_PRC) {
			cmds[prc_idx].argv[arg_idx] = NULL;
			prc_idx++;
			arg_idx = 0;
		} else if (result == RS_TOK)
			tok_idx++;
		else if (result == RS_ARG) {
			if (!cmds[prc_idx].path)
				cmds[prc_idx].path = tokens[tok_idx];
			else
				cmds[prc_idx].argv[arg_idx++] = tokens[tok_idx];
		}
	}
	return 0;
}

void close_fds(cmd_t *p) {
	for (int i = 0; i < MAX_CMD_SIZE && p->fdclose[i] != -1; ++i) {
		close(p->fdclose[i]);
		p->fdclose[i] = -1;
	}
}

int exec_cmd(cmd_t *p) {
	pid_t pid = fork(); // Création d'un nouveau processus fils (fork)
	if (pid < 0)
		return -1;
	if (pid) {
		// child
		if (p->stdin != STDIN_FILENO) {
			dup2(p->stdin, STDIN_FILENO);
		}
		if (p->stdout != STDOUT_FILENO) {
			dup2(p->stdout, STDOUT_FILENO);
		}
		if (p->stderr != STDERR_FILENO) {
			dup2(p->stderr, STDERR_FILENO);
		}
		close_fds(p); // redirections are done ! fds are not longer needed
		exit(execvp(p->path, p->argv) == -1);
	} else {
		// prent
		p->pid = pid;
		/*if (p->wait){
			if (waitpid(pid, &p->status, 0) == -1) return -1 ;
		}else */
		if (!p->wait)
			printf("proc %d in background\n", pid);
		close_fds(p);
	}
	return 0;
}
