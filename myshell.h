#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#define MAXARGS   128
#define	MAXLINE	 8192  /* Max text line length */

void eval(char *cmdline);
int parseline(char *buf, char **argv);

/* Our own error-handling functions */
void unix_error(char *msg);

/* Process control wrappers */
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);

/* cd command functions */
void Chdir(char *argv);
void Cd(char **argv);

/* echo command functions */
int chk_echo(char **argv);

#endif
