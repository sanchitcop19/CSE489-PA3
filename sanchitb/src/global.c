#include <string.h>
#include "../include/global.h"


/*https://stackoverflow.com/questions/7863499/conversion-of-char-to-binary-in-c*/
char* char2bits(char c)
{  
    char* final = malloc(8*sizeof (char)); 
    char* temp = malloc(sizeof (char));
	memset(final, '\0', 8*sizeof(char));
    for (int i = 7; i >= 0; --i)
    {  
	temp =  ((c & (1 << i)) ? "1" : "0" );
        strcat(final, temp);
    }
    return final;
}
pair get_init_info(int bits, char* cntrl_payload, int payload_len){
	int i;
	char* num_routers = strcat(char2bits(cntrl_payload[0]), char2bits(cntrl_payload[1]));
	unsigned long num_r = strtol(num_routers, NULL, 2);
	printf("num_routers: %u\n", num_r);
	char* update_b = strcat(char2bits(cntrl_payload[2]), char2bits(cntrl_payload[3]));
	unsigned long update = strtol(update_b, NULL, 2);
	printf("update interval: %u\n", update);
	pair info = {num_r, update};
	return info;
}
pair get_info(int bits, char* cntrl_payload, int last, int control_code, int payload_len){
	return get_init_info(bits, cntrl_payload, payload_len);
}
