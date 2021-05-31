#include "myshell.h"  

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

	pid_t pid;           /* Process id */   
    	strcpy(buf, cmdline);
	parseline(buf, argv); 
    
    	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */

    	if(!strcmp(argv[0],"ls")){
		pid = fork();
		if(pid == 0){
			Execve("/bin/ls",argv,NULL);
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
		Cd(argv);
	}
	else if(!strcmp(argv[0],"mkdir")){
		pid = fork();
		if(pid == 0){
			Execve("/bin/mkdir",argv,NULL);;
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
			Execve("/bin/rmdir",argv,NULL);
			exit(0);	
		}
		else if(pid > 0){
			Wait(&child_status);
		}
		else
			unix_error("Fork error");
	}
	else if(!strcmp(argv[0],"touch")){
		pid = fork();
		if(pid == 0){
			Execve("/bin/touch",argv,NULL);
			exit(0);	
		}
		else if(pid > 0){
			Wait(&child_status);
		}
		else
			unix_error("Fork error");
	}
	else if(!strcmp(argv[0],"cat")){
		pid = fork();
		if(pid == 0){
			Execve("/bin/cat",argv,NULL);
			exit(0);	
		}
		else if(pid > 0){
			Wait(&child_status);
		}
		else
			unix_error("Fork error");	
	}
	else if(!strcmp(argv[0],"echo")){
		int flag = chk_echo(argv);
		if (flag == 0){
			pid = fork();
			if(pid == 0){
				Execve("/bin/echo",argv,NULL);
				exit(0);	
			}
			else if(pid > 0){
				Wait(&child_status);
			}
			else
				unix_error("Fork error");	
		}
		else{
			unix_error("echo fail");	
		}	
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
	else{
		if (execve(argv[0], argv, NULL) < 0) {	//ex) /bin/ls ls -al &
            		printf("%s: Command not found.\n", argv[0]);
			exit(0);
        	}
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

void Chdir(char *argv){
	char msg[1000] = "cd: ";
	strcat(msg,argv);
	if(chdir(argv) == -1)
		unix_error(msg);
}
void Cd(char **argv){
	int i, flag = 0;
	char path[1000];
	memset(path, '\0',sizeof(path));
	
	if(argv[1] == NULL){
		Chdir(getenv("HOME"));
	}
	else if(argv[1][0] == '~') {
		flag = 0;
		for(i = 1; i < strlen(argv[1]); i++){
			path[i-1] = argv[1][i];
			flag = 1;
		}
		strcat(path,"\0");
		Chdir(getenv("HOME"));
		if(flag == 1){
			Chdir("..");
			Chdir(path);
		}
	}
	else if(argv[1][0] == '$'){
		for(i = 1; i < strlen(argv[1]); i++){
			path[i-1] = argv[1][i];
		}
		strcat(path,"\0");
		Chdir(getenv(path));
	}
	else{
		Chdir(argv[1]);	
	}
}
int chk_echo(char **argv) {
	
	char buf[1000];
	memset(buf, '\0',sizeof(buf));
	int i, flag = 0;
	
	/* "" parsing */
	if(argv[1][0] == '"'){
		flag = 1;
		int j = 0;
		/* 환경변수 처리 */
		if(argv[1][1] == '$'){
			for(i = 2; i < strlen(argv[1]); i++){
				if(argv[1][i] == '"')
					flag ++;
				else 
					buf[i-2] = argv[1][i];
			}
			buf[i] = '\0';
			strcpy(argv[1],getenv(buf)); 
		}
		else{
			for(i = 1; i < strlen(argv[1]); i++){
				if(argv[1][i] == '"'){
					flag ++;
				}
				else
					buf[j++] = argv[1][i];
			}
			buf[j] = '\0';
			strcpy(argv[1],buf); 	
		}
	}
	/* 환경변수 처리 */
	else if(argv[1][0] == '$'){
		for(i = 1; i < strlen(argv[1]); i++){
			buf[i-1] = argv[1][i];
		}
		buf[i] = '\0';
		strcpy(argv[1],getenv(buf)); 
	}
	/* '' parsing */
	else if(argv[1][0] == 39){
		flag = 1;
		int j = 0;
		for(i = 1; i < strlen(argv[1]); i++){
			if(argv[1][i] == 39){
				flag ++;
			}
			else
				buf[j++] = argv[1][i];
		}
		buf[j] = '\0';
		strcpy(argv[1],buf); 	
	}
	
	/* 따옴표가 정상적이지 않을 경우*/
	if(flag == 0 || flag % 2 == 0){
		return 0;
	}
	else{
		return -1;
	}
}
