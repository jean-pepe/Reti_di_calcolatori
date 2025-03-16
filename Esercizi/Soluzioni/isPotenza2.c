//dato un numero x ---> è esso una potenza di 2?
#include<stdio.h>

int isPotenza2(unsigned int x){

	int count = 0;
	for(int i =0; i < sizeof(x) * 8; i++){
		int bitInteresse = x & (1 <<i);
		if(bitInteresse)
			count++;
	}


	return count == 1;
}


int main(){

 unsigned int x = 1022;

 if(isPotenza2(x))
	 printf("x e' potenza di 2\n");
 else
	 printf("x non e' potenza di 2\n");

}
