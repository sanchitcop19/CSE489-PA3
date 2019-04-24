#include <string.h>
#include "../include/global.h"

void initialize_neighbors(int num_r){
	for (int i = 0; i < num_r; ++i){
        	neighbors[i] = NULL;
	}
}
int num_digits(long n){
    int count = 0;


    while(n != 0)
    {
          n /= 10;
                  ++count;
                             }
   return count;     
}

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

pair get_update_info(char* cntrl_payload){
	char* id_b = strcat(char2bits(cntrl_payload[0]), char2bits(cntrl_payload[1]));
	unsigned long id = strtol(id_b, NULL, 2);
	printf("router update id: %u\n", id);	
	
	char* cost_b = strcat(char2bits(cntrl_payload[2]), char2bits(cntrl_payload[3]));
	unsigned long cost = strtol(cost_b, NULL, 2);
	printf("router update cost: %u\n", cost);	
	pair result = {id, cost};
	return result;
}

receiver get_sendfile_info(char* payload){
	char* ip_b = strcat(char2bits(payload[0]), char2bits(payload[1]));
	ip_b = strcat(ip_b, char2bits(payload[2]));
	ip_b = strcat(ip_b, char2bits(payload[3]));
	printf("ip: %s", ip_b);
	receiver temp;
	return temp;


}

pair get_info(int bits, char* cntrl_payload, int last, int control_code, int payload_len){
	if (control_code == 1)return get_init_info(bits, cntrl_payload, payload_len);
	else if (control_code == 3)return get_update_info(cntrl_payload);
}
void print_map(uint_map_t map){
	const char *key;
        map_iter_t iter = map_iter(&map);

        while ((key = map_next(&map, &iter))) {
                printf("%s -> %d\n", key, *map_get(&map, key));
        }
}
void print_dv(){
	for (int i = 0; i < 5; ++i){
		for (int j = 0; j < 5; ++j){
			unsigned int cost = dv[i][j];
			int digits = num_digits(dv[i][j]);
			printf(" %i ", cost);
			for (int temp = digits; temp < 5; ++temp)printf(" ");
		}
		printf("\n");
	} 
}

