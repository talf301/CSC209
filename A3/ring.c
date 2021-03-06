#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int n;
int *pipefds;
int main(int argc, char **argv)
{
	extern void zeroprocess();
	extern void iprocess(int i);

	int i;
	if(argc != 2 || (n = atoi(argv[1])) < 2) {
		fprintf(stderr, "usage: %s ringsize\n", argv[0]);
		fprintf(stderr, "ring size must be at least 2\n");
		return(1);
	}

	if((pipefds = malloc(2*n*sizeof(int))) == NULL) {
		perror("malloc");
		return(1);
	}
	
	for(i = 0; i < n; i++) {
		if(pipe(pipefds + 2*i) < 0) {
			perror("pipe");
			return(1);
		}
	}

	for(i = 0; i < n-1; i++) {
		switch(fork()) {
		case -1:
			perror("fork");
			return(1);
		/*processes 0...n-2 (specifically process i)*/
		case 0:
			if(i == 0)
				zeroprocess();

			iprocess(i);
		default:
			/*process n-1*/
			if(i == n-2) {	
				iprocess(n-1);
			}
		}
	}

	return(0);
}

void iprocess(int i)
{
	//correct process indexing
	i -= 1;

	int j;
	char buf[1000];
	/*close all pipes except 2*i and 2*i +3*/
	for(j = 0; j < 2*n; j++) {
		if(j != 2*i && j != 2*i + 3) {
			close(pipefds[j]);
		}
	}

	while(1) {
		switch(read(pipefds[2*i], buf, 1000)) {
		case -1:
			perror("read");
			exit(1);
			break;
		case 0:
			printf(
"process #%d (%d) exiting due to EOF on frompipe\n", i+1, getpid());
			exit(0);
			break;

		default:
			buf[0]++;
			printf(
"process #%d (%d) relaying message: %s\n", i+1, getpid(), buf);
			if(write(pipefds[2*i+3], buf, 1000) < 0) {
				perror("write");
				exit(1);
			}
			break;
		}
	}

}

void zeroprocess()
{
	char *p;
	char buf[1000];
	int j;
	/*close all pipes except 1 and 2*n-2*/
	for(j = 0; j < 2*n; j++) {
		if(j != 1 && j != 2*n-2) {
			close(pipefds[j]);
		}
	}

	while(1) {
		if(fgets(buf, 1000, stdin) == NULL) {
			//NULL could mean an error or end of file
			if(!feof(stdin)) {
				perror("fgets");
				exit(1);
			}
			printf(
"process #0 (%d) exiting due to EOF on stdin\n", getpid());
			close(pipefds[1]);
			close(pipefds[2*n-2]);
			exit(0);
		}
		
		if((p = strchr(buf, '\n')))
			*p = '\0';
		printf(
"process #0 (%d) sending message: %s\n", getpid(), buf);
		if(write(pipefds[1], buf, 1000) < 0) {
			perror("write");
			exit(1);
		}
		if(read(pipefds[2*n-2], buf, 1000) < 0) {
			perror("read");
			exit(1);
		}
		printf("I hear %s\n", buf);
	}

}
