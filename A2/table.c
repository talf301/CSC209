#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 500

static int count  = 0;

int main(int argc, char **argv)
{
	int h, status = 0;
	FILE *fp;
	extern void process(FILE *fp);

	while((h = getopt(argc, argv, "h:")) != EOF)
	{
		switch(h)  {
		case 'h':
			count = atoi(optarg);
			break;
		case '?':
		default:
			status = 1;
			break;
		}
	}
	
	if(status)
	{
		fprintf(stderr,"usage: %s [-h count] [FILE]...\n", argv[0]);
	}

	printf("<table>\n");
	
	/*Using standard input*/
	if(optind >= argc)
	{
		process(stdin);
	}
	/*Using files*/
	else
	{
		for(; optind < argc; optind++)
		{
			if(strcmp(argv[optind], "-") == 0) {
				process(stdin);
			} else if((fp = fopen(argv[optind], "r")) == NULL) {
				perror(argv[optind]);
				exit(1);
			} else {
				process(fp);
				fclose(fp);
			}
		}
	}
	printf("</table>");
	return(status);
}

void process(FILE *fp)
{
	char a[MAXLINE];
	int i = 0;
	int newline = 1;
	int header = 'd';
	/*loop will break when fgets() returns null*/
	while(fgets(a, MAXLINE, fp) != NULL)
	{
		if(newline) {
			if(count > 0) {
				header = 'h';
				count--;
			} else {
				header = 'd';
			}
			printf("<tr><t%c>", header);
			newline = 0;
		}
		i = 0;
		while(a[i] != '\0')
		{
			switch(a[i]) {
			case '\t':
				printf("</t%c><t%c>", header, header);
				break;
			case '\n':
				newline = 1;
				printf("</t%c></tr>\n", header);
				break;
			default:
				printf("%c", a[i]);
				break;
			}
			i++;
		}
	}
}
