#include <stdio.h>
#include <inttypes.h>

int main( int argc, const char* argv[] ){
	uint8_t myRegister = 0b00101111; // represents 47
	printf("1st print %u \n", myRegister);
	myRegister |= (1<<6);
	printf("2nd print %u \n", myRegister);
	myRegister &= ~(1<<3);
	printf("3rd print %u \n", myRegister);
	printf("%u\n",1u);
	printf("%u\n",0b00010101 << 2);
}
// 2nd print
// 1 = 0000 0001
// 1 << 6 = 0100 0000
// myRegister = 0010 1111
// 1 << 6     = 0100 0000
// |= 		  = 0110 1111 
// 3rd print
// 1      = 0000 0001
// 1 << 3 = 0000 1000
// ~(1<<3)    = 1111 0111
// myRegister = 0110 1111
// &=         = 0110 0111


