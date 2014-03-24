#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

static int status = 0;
int main(int argc, char **argv)
{
	DIR *dp;
	struct dirent *p;
	extern void listdir(char *path);
	if(argc == 1)
	{
		if((dp = opendir(".")) == NULL) {
			perror(".");
			return(1);
		}
		
		while((p = readdir(dp)))
		{
			if(strcmp(p->d_name,".")!=0 && strcmp(p->d_name, "..") 
!=0) {
				printf("%s\n", p->d_name);
				listdir(p->d_name);
			}
		}
	}
	int i;
	for(i = 1; i < argc; i++)
	{
		listdir(argv[i]);
	}

	return(status);
}

void listdir(char *path)
{
	DIR *dp;
	struct dirent *p;
	struct stat q;

	if((dp = opendir(path)) == NULL) {
		perror(path);
		status = 1;
		return;
	}

	while((p = readdir(dp)))
	{
		char a[strlen(path)+strlen(p->d_name)+1];
		strcpy(a, path);
		strcat(a, "/");
		strcat(a, p->d_name);
		lstat(a, &q);
		if(strcmp(p->d_name,".")!=0 && strcmp(p->d_name,"..")!=0)
		{
			printf("%s/%s\n", path, p->d_name);
			if(S_ISDIR(q.st_mode))
			{
				listdir(a);
			}
		}
	}
}
