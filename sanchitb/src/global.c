#include <string.h>
#include <limits.h>
#include <arpa/inet.h>
#include "../include/global.h"
#define OFF 0x0c

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

router* get_router(uint32_t ip){
	for (int z = 0; z < _numr; ++z){
		if (routers[z]->ip == ip)return routers[z];
	}	
	return NULL;
}
const char* get_id_from_index(int col){
	const char *key;
	map_iter_t iter = map_iter(&index_map);
	while (key = map_next(&index_map, &iter)){
		if (*map_get(&index_map, key) == col)return key;
	}
}
void bellman_ford(){
	for(int n = 0; n < _numneighbors; ++n){
		unsigned long nid = neighbors[n];
		char snid[50];
		memset(snid, '\0', sizeof (snid));
		sprintf(snid, "%u", nid);
		unsigned long index = *map_get(&index_map, snid);
		uint16_t self_to_other_cost;
		uint16_t other_cost;
		uint32_t other_ip;
		uint16_t total;
		for (int col = 0; col < _numr; ++col){
			self_to_other_cost = *map_get(&weight_map, snid);
			other_cost = dv[index][col];
			if (other_cost == USHRT_MAX || self_to_other_cost == USHRT_MAX)total = USHRT_MAX;
			else total = other_cost + self_to_other_cost;
			if (dv[_row][col] > total){
				dv[_row][col] = other_cost + self_to_other_cost;
				other_ip = *map_get(&ip_map, snid);
				const char* updateid = get_id_from_index(col);
				//router* rout = get_router(other_ip);
				//rout->
				map_set(&next_hop, updateid, nid);
				map_set(&weight_map, updateid, total);
			}
		}
	}	
}

void process_dv(char* data){
	char* numupdates = strcat(char2bits(data[0]), char2bits(data[1]));	
	uint16_t num_updates = strtol(numupdates, NULL, 2);
	printf("num_updates: %u\n", num_updates);

	char* interval = strcat(char2bits(data[2]), char2bits(data[3]));	
	uint16_t uinterval = strtol(interval, NULL, 2);
	//uinterval = ntohs(uinterval);
	
	char* sourceip = strcat(char2bits(data[4]), char2bits(data[5]));	
	strcat(sourceip, char2bits(data[6]));
	strcat(sourceip, char2bits(data[7]));
	uint32_t sip = strtol(sourceip, NULL, 2);
	//sip = ntohl(sip);
	//
		const char * key;
		unsigned int temprow = USHRT_MAX;
		map_iter_t iter = map_iter(&ip_map);
		while(key = map_next(&ip_map, &iter)){
			if (*(map_get(&ip_map, key)) == sip){
				temprow = *(map_get(&index_map, key));	
			}
		}

	char* srcip;
	char* port;
	char* id;
	char* cost;
	uint32_t usrcip;
	uint16_t uport;
	uint16_t uid;
	uint16_t ucost;
	for (uint16_t counter = 0; counter < num_updates; ++counter){
		uint16_t index = counter*OFF + 0x08;
			
		srcip = strcat(char2bits(data[index]), char2bits(data[index + 1]));	
		strcat(srcip, char2bits(data[index + 2]));
		strcat(srcip, char2bits(data[index + 3]));
		usrcip = strtol(srcip, NULL, 2);
	//	usrcip = ntohl(usrcip);
		port = strcat(char2bits(data[index+4]), char2bits(data[index+5]));	
		uport = strtol(port, NULL, 2);
	//	uport = ntohs(uport);
	
		id = strcat(char2bits(data[index+8]), char2bits(data[index+9]));
		uid = strtol(id, NULL, 2);
	//	uid = ntohs(uid);
		char idasstring[50];
		memset(idasstring, '\0', sizeof(idasstring));
		sprintf(idasstring, "%u", uid);
		unsigned int tempcol = *map_get(&index_map, idasstring);	
			
		cost = strcat(char2bits(data[index+10]), char2bits(data[index+11]));
		ucost = strtol(cost, NULL, 2);
	//	ucost = ntohs(ucost);
		printf("updating dv[%u][%u] to %u\n", temprow, tempcol, ucost);
		dv[temprow][tempcol] = ucost;
		print_dv();						

	}	
	
}
