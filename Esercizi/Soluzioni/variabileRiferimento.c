#include<stdio.h>

void changeX(char* x){
	*x = 15;
	printf("address x:%p\n", &x);
}

int main(){


	char a = 10;

	printf("a:%d\n", a);
	printf("address a:%p\n", &a);

	changeX(&a);
	
	printf("a:%d\n", a);
}
