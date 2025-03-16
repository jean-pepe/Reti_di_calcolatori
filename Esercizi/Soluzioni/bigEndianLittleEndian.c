#include <stdio.h>


int main(){

	unsigned int a = 0xff000000;

	unsigned char *p;

	p = (unsigned char*) &a;

	if(*p == 0)
		printf("little endian");
	else
		printf("big endian");
	printf("%d\n", *p);

}
