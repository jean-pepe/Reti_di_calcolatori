#include <stdio.h>

int main(){

	unsigned short int a;

	unsigned short int bit = 1;
	a = 15;

	for(int i=  8 * sizeof(short int) - 1; i >= 0; i--){
		short int bitInteresse = a & ( bit << i);
		if(!bitInteresse)
			printf("0");
		else 
			printf("1");
	}

	printf("\n");
}
