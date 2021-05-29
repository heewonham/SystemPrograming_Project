#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH

#define MAXARGS   128
#define	MAXLINE	 8192  /* Max text line length */

typedef struct sockaddr SA;

#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

extern int h_errno;    /* Defined by BIND for DNS errors */ 
extern char **environ; /* Defined by libc */

#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
void unix_error(char *msg){
	
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
void Execve(const char *filename, char *const argv[], char *const envp[]) {
	
    if (execve(filename, argv, envp) < 0)
		unix_error("Execve error");
}
pid_t Wait(int *status) {
	
    pid_t pid;

    if ((pid  = wait(status)) < 0)
		unix_error("Wait error");
    return pid;
}

int main() {
	
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
		/* Read */
		printf("CSE4100:P4-myshell> ");                   
		fgets(cmdline, MAXLINE, stdin); 
		if (feof(stdin))
		    exit(0);

		/* Evaluate */
		eval(cmdline);
    } 
}

/* $begin eval - Evaluate a command line */
void eval(char *cmdline) {
	
	char *argv[MAXARGS]; /* Argument list execve() */
	char buf[MAXLINE];   /* Holds modified command line */
	int child_status;
	int bg;              /* Should the job run in bg or fg? */
	pid_t pid;           /* Process id */
    
    	strcpy(buf, cmdline);
    	bg = parseline(buf, argv); 
 
    	printf("%d\n",bg);   
    	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */

    	if(!strcmp(argv[0],"ls")){
		pid = fork();
		if(pid == 0){
			execve("/bin/ls",argv,NULL);
			exit(0);
		}
		else if(pid > 0){
			Wait(&child_status);
		}
		else{
			unix_error("Fork error");
		}	
	}
	else if(!strcmp(argv[0],"cd")){
		if(argv[1] == NULL){
			if(chdir(getenv("HOME")) == -1)
				perror("chdir() failed.");
		}
		else if(chdir(argv[1]) == -1)
			perror("chdir() failed.");
	}
	else if(!strcmp(argv[0],"mkdir")){
		pid = fork();
		if(pid == 0){
			execlp(argv[0],argv[0],argvp[1],NULL);
			exit(0);	
		}

		else if(pid > 0){
			Wait(&child_status);
		}
		else
			unix_error("Fork error");
	}
	else if(!strcmp(argv[0],"rmdir")){
		pid = fork();
		if(pid == 0){
			execlp(argv[0],argv[0],argvp[1],NULL);
			exit(0);	
		}
		else if(pid > 0){
			Wait(&child_status);
		}
		else
			unix_error("Fork error");
	}
	else if(!strcmp(argv[0],"exit")){
		pid = fork();
		if(pid == 0)
			exit(0);
		else if(pid > 0){
			Wait(&child_status);
			exit(0);
		}
		else
			unix_error("Fork error");
	}
    return;
}

/* $begin parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) {
	
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
		buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* Ignore spaces */
         	buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
		return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

    return bg;
}
