#include <stdio.h>
#include <string.h>
int main()
{	
	char a[20];
	strcpy(a, "Hello");
	
	printf("swag: %s", strcat(a, " two"));
	return(0);
}
