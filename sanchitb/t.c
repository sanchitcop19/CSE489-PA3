#include <stdio.h>
#include <inttypes.h>
#define OFF 0x0c

int main(){
	uint16_t index = 0*OFF + 0x08;
	printf("index: %u\n", index);
	index = 1*OFF + 0x08;
	printf("index: %u\n", index);
	return 0;
}
