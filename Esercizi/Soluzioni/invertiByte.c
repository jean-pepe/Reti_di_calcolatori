//dato x = 0x12345678 --> 0x78563412
//

#include <stdio.h>

int main(){

	unsigned int a = 0x12345678;

	printf("%x\n", a);


	unsigned int a1 = a & 0x000000ff;
	unsigned int a2 = a & 0x0000ff00;
	unsigned int a3 = a & 0x00ff0000;
	unsigned int a4 = a & 0xff000000;

	printf("%x\n", a1);
	printf("%x\n", a2);
	printf("%x\n", a3);
	printf("%x\n", a4);

	unsigned bigEndian = (a1 << 24) | (a2 << 8) | (a3 >> 8) | (a4 >> 24);

	printf("Big endian:%x\n", bigEndian);
	
}

