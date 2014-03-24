#include <stdio.h>
#include "parse.h"

int main()
{
    char buf[1000], **p;

    while (printf("$ "), fgets(buf, sizeof buf, stdin)) {
        if ((p = parse(buf))) {
	    for (; *p; p++)
		printf("\"%s\"\n", *p);
	}
    }

    return(0);
}
