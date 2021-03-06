#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	if(argc == 1) {
		fprintf(stderr, "usage: %s FILE...\n", argv[0]);
		return(1);
	}

	int i;
	for(i = 1; i < argc; i++)
	{
		int fd;
		if((fd = open(argv[i], O_RDWR|O_CREAT, 0666)) == -1) {
			perror(argv[i]);
			return(1);
		}
		
		char a[1];
		int numread;
		numread = read(fd, a, 1);
		/*Error case*/	
		if(numread == -1) {
			perror(argv[i]);
			return(1);
		} else if(numread == 0) {
		/*Empty case*/
			write(fd, "1", 1);
			ftruncate(fd, 0);
		} else if(numread == 1) {
		/*Normal case*/
			lseek(fd, 0, SEEK_SET);
			write(fd, a, 1);
		}
		close(fd);
		
	}
	return(0);
}
