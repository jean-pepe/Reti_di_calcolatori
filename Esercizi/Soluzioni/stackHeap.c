#include <stdio.h>
#include <stdlib.h>

int x,y,z;
int main(){

	x =1, y = 2, z = 3;

	printf("x:%p\n", &x);
	printf("y:%p\n", &y);
	printf("z:%p\n", &z);

	int a,b,c;
	a = 12;b=13;c=14;

	printf("a:%p\n", &a);
	printf("b:%p\n", &b);
	printf("c:%p\n", &c);

	int *p1 = (int*)malloc(4);
	int *p2 = (int *)malloc(4);
	printf("p1:%p\n", p1);
	printf("p2:%p\n", p2);
	printf("addr p1:%p\n", &p1);
	printf("addr p2:%p\n", &p2);
}
