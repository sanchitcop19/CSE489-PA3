/**
 * @author
 * @author  Swetank Kumar Saha <swetankk@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * INIT [Control Code: 0x01]
 */

#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/map.h"
#include "../include/control_handler.h"
#include "../include/queue.h"

int num_routers = 5;

void initialize_dv(int num){
	const char *key;
	unsigned int i = 0;
	for (int m = 0; m < 5; ++m){
		for (int n = 0; n < 5; ++n){
			dv[m][n] = USHRT_MAX;
		}
	}	
	unsigned int row = USHRT_MAX;
        map_iter_t iter = map_iter(&weight_map);
        while ((key = map_next(&weight_map, &iter))) {
		printf("Mapping id %s to index %i\n", key, i);
		map_set(&index_map, key, i);
		printf("weight for self == 0: %u\n", *map_get(&weight_map, key));
		if (*map_get(&weight_map, key) == 0) row = i;	
		i++;
        }
	iter = map_iter(&index_map);
	print_map(weight_map);
        while ((key = map_next(&index_map, &iter))) {
		unsigned int index = *map_get(&index_map, key);
		unsigned int cost = *map_get(&weight_map, key);
		dv[row][index] = cost;
	}
	print_dv();
}

void generate_response(int sock_index){
	uint16_t payload_len, response_len;
	char* cntrl_response_header;

	payload_len = 0;
	cntrl_response_header = create_response_header(sock_index, 1, 0, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE + payload_len;
	
	sendALL(sock_index, cntrl_response_header, response_len);

}
void build_adj_list(){

/*converts the data structure formed form convert_payload (list of info) to adjacency list*/
	char buf[2];

	map_init(&weight_map);
	map_init(&ip_map);	
	map_init(&port_router_map);	
	map_init(&port_data_map);	
	map_init(&next_hop);
	map_init(&index_map);
	map_init(&sock_map);
	router** trav = routers;
	for (int i = 0; i < _numneighbors; ++i, ++trav){

		memset(buf, '\0', sizeof buf);
		sprintf(buf, "%u", (*trav)->id);

		map_set(&weight_map, buf, (*trav)->cost);
		
		map_set(&ip_map, buf, (*trav)->ip);
		
		map_set(&port_router_map, buf, (*trav)->port1);
		
		map_set(&port_data_map, buf, (*trav)->port2);

		map_set(&next_hop, buf, (*trav)->next_hop);
	}
	const char *key;
        map_iter_t iter = map_iter(&weight_map);

        while ((key = map_next(&weight_map, &iter))) {
                printf("%s -> %d\n", key, *map_get(&weight_map, key));
        }
} 

void init_response(int sock_index, char* cntrl_payload, int payload_len){
//Assuming self to be euston
//Save routers at the id'th position in the array
pair info = get_info(16, cntrl_payload, 0, 1, payload_len);
int num_r = info.x;
_numr = num_r;
_numneighbors = 0;
update_interval = info.y;
printf("update interval: %i\n", update_interval);
char* iter = cntrl_payload + 4;


initialize_neighbors(num_r);
int temp = 0;
printf("------------------------------------------------\n");
get_ip();
routers = malloc((sizeof(router*)*_numr));
router** trav = routers;

for (int c = 0, i = 0; c < num_r; ++c, iter += 12){

	char* id_b = strcat(char2bits(iter[i]), char2bits(iter[i+1]));
	unsigned long id = strtol(id_b, NULL, 2);
	printf("id of router %i: %u\n", c, id);
			
	char* port1_b = strcat(char2bits(iter[i+2]), char2bits(iter[i+3]));
	unsigned long port1 = strtol(port1_b, NULL, 2);
	printf("port1 of router %i: %u\n", c, port1);

	
	char* port2_b = strcat(char2bits(iter[i+4]), char2bits(iter[i+5]));
	unsigned long port2 = strtol(port2_b, NULL, 2);
	printf("port2 of router %i: %u\n", c, port2);

	char* cost_b = strcat(char2bits(iter[i+6]), char2bits(iter[i+7]));
	unsigned long cost = strtol(cost_b, NULL, 2);
	printf("cost of router %i: %u\n", c, cost);

	char* ip_b = strcat(char2bits(iter[i+8]), char2bits(iter[i+9]));
	char* tmp = strcat(char2bits(iter[i+10]), char2bits(iter[i+11]));
	ip_b = strcat(ip_b, tmp);
	printf("ip of router %i in binary: %s\n", c, ip_b);
	unsigned long ip = strtol(ip_b, NULL, 2);
	router* _router = malloc(sizeof(router));
	if (ip == ip_l){
		router_port = port1;
		data_port = port2;
	}
	printf("ip of router %i: %u\n", c, ip);
	printf("router port: %u, data port: %u\n", router_port, data_port);
	*trav = _router;
	(*trav)->id = id;
	(*trav)->port1 = port1;
	(*trav)->port2 = port2;
	(*trav)->cost = cost;
	(*trav)->ip = ip;
	(*trav)->strike = 0;
	printf("neighbors: ");
	if (cost == USHRT_MAX)(*trav)->next_hop = USHRT_MAX;
	else {
		neighbors[temp] = id;
		_numneighbors++;		
		printf("%u ", id);
		temp++;
		(*trav)->next_hop = id; 
	}
	if (ip == ip_l){self_id = id;self_ip = ip;
	
		time_t now = time(NULL);        
		printf("current time: %u\n", now);
		struct timeval* temp = malloc(sizeof(struct timeval));
		temp->tv_sec = now + update_interval;
		temp->tv_usec = 0;
		printf("timeout: %u\n", temp->tv_sec);
                timeout_qpair *self = malloc(sizeof(timeout_qpair));
		self->r = _router;
		self->to = temp;
                push(self);
		printq();
                timeout.tv_sec = update_interval;
                timeout.tv_usec = 0;
	}
	trav++;
	printf("\n");
printf("------------------------------------------------\n");
}
build_adj_list();
initialize_dv(num_r);
generate_response(sock_index);
create_router_sock();
printq();
};
