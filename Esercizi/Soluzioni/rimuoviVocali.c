//data stringa ciao a tutti! --> c  ttt!
//
#include<stdio.h>

int isVocale(char c){

	return (c == 'a' || c == 'i' || c == 'o' || c == 'e' || c == 'u'||
	 c == 'A' || c == 'I' || c == 'O' || c == 'E' || c == 'U');
}

void rimuoviVocali(char *s){
	int j = 0;
	for(int i = 0; s[i]; i++){
		if(!isVocale(s[i])){
			s[j] = s[i];
			j++;
		}
	}
	s[j] = 0;	
	return;
}

int main(){

	char s[] = "ciao a tutti!";

	char s1[] = {'c','i', 'a', 'o'};

	printf("%s\n", s);

	rimuoviVocali(s);

	printf("%s\n", s);


}
