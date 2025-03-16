#include <stdio.h>

void print_u16(short int a) {
	for (int i = 0; i < 8 * sizeof(short int); i++) {
		short int bitInterest = (a & (1 << i)) != 0;
		printf("%d", bitInterest);
	}
	printf(" | ");
}
int main1() {
	char c = 'A';	  // 1 byte of memory
	printf("%c \n", c);// c = A  (read as a char)
	printf("%d \n", c);// c = 65 (read as a int)
	printf("%x \n", c);// c = 41 (read as a hex)
	/*					         VARIABLE     VALUE     ADRESS
	*								c -> |0X 0100 0001| $ 4001
	*									 |            | $ 4002
	* 									 |            | $ 4003
	*/
	char* p;//char pointer
	p = &c;
	*p = 'z';// We edit C without naming C
	printf("%c \n", c);

	short int a[3] = { 0,0,0 };//ARRAY == continuous block of memory
	/*					           VARIABLE      VALUE     ADRESS
	*								a[0] -> |0X 0000 0000| $ 4001
	*									    |0X 0000 0000| $ 4002
	*								a[1] -> |0X 0000 0000| $ 4003
	*									    |0X 0000 0000| $ 4004
	* *								a[2] -> |0X 0000 0000| $ 4005
	*									    |0X 0000 0000| $ 4006
	*/
	print_u16(a[0]);
	print_u16(a[1]);
	print_u16(a[2]);
	printf("\n");
	short int* p1;
	p1 = &a[0];
	*(p1 + 1) = 0xFFFF; //pointer arithmetic --> I modify adjacent memory cell
	//*(p1 + 1) == &p1 + 1*(sizeof(
	/*					           VARIABLE      VALUE     ADRESS
	*								a[0] -> |0X 0000 0000| $ 4001
	*									    |0X 0000 0000| $ 4002
	*								a[1] -> |0X 1111 1111| $ 4003
	*									    |0X 1111 1111| $ 4004
	* *								a[2] -> |0X 0000 0000| $ 4005
	*									    |0X 0000 0000| $ 4006
	*/
	print_u16(a[0]);
	print_u16(a[1]);
	print_u16(a[2]);
	printf("\n");
	printf("Size of Array %d", sizeof(a) / sizeof(a[0]));//size of Array
	return 0;
}