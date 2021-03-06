#include <stdio.h>
#include <string.h>
#include "parse.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

int laststatus = 0;

int main()
{
    char buf[1000], **p;
    extern void execute(char **argv);

    while (printf("$ "), fgets(buf, sizeof buf, stdin))
        if ((p = parse(buf)))
            execute(p);

    return(laststatus);
}

void execute(char **argv)
{
	extern char **environ;
	struct stat statbuf;
	char *cpath;
	if((cpath = malloc(20+strlen(argv[0]) * sizeof(char))) == 
NULL) {
		perror("malloc");
		exit(1);
	}
	/*if we reach the end of this if statement, cpath contains an existing file which we are going to attempt to execute*/
	if(strchr(argv[0], '/') == NULL) {
		strcpy(cpath, "/bin/");
		strcat(cpath, argv[0]);
		if(!stat(strcat(strcpy(cpath,"/bin/"),argv[0]),&statbuf)){}	
		else if(!stat(strcat(strcpy(cpath,"/usr/bin/"),argv
[0]),&statbuf)){}
		else if(!stat(strcat(strcpy(cpath,"/usr/local/bin/"), argv
[0]), &statbuf)){}
		else if(stat(strcat(strcpy(cpath, "./"),argv[0]), &statbuf))
		{
			fprintf(stderr, "%s: Command not found\n", argv[0]);
			return;
		}	
	} else {
		if(stat(strcpy(cpath, argv[0]),&statbuf)) {
			fprintf(stderr, "%s: Command not found\n", argv[0]);
			return;
		}
	}
	int x;
	if((x = fork()) < 0)
	{
		perror("fork");
		return;
	}
	if(x == 0) {
		execve(cpath, argv, environ);
		perror(cpath);
		return;
	} else {
		if(wait(&laststatus) < 0) {
			perror("wait");
			exit(1);
		}
		laststatus = laststatus >> 8;
		free(cpath);
		return;
	}
}
