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
#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/map.h"

int num_routers = 5;

void generate_response(int sock_index){
	uint16_t payload_len, response_len;
	char* cntrl_response_header;

	payload_len = 0;
	cntrl_response_header = create_response_header(sock_index, 1, 0, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE + payload_len;
	
	sendALL(sock_index, cntrl_response_header, response_len);

}
void build_adj_list(router* routers[]){

/*converts the data structure formed form convert_payload (list of info) to adjacency list*/
	char buf[2];

	map_init(&weight_map);
	map_init(&ip_map);	
	map_init(&port_router_map);	
	map_init(&port_data_map);	
	map_init(&next_hop);
	for (int i = 0; i < num_routers; ++i){

		memset(buf, '\0', sizeof buf);
		sprintf(buf, "%u", routers[i]->id);

		map_set(&weight_map, buf, routers[i]->cost);
		
		map_set(&ip_map, buf, routers[i]->ip);
		
		map_set(&port_router_map, buf, routers[i]->port1);
		
		map_set(&port_data_map, buf, routers[i]->port2);

		map_set(&next_hop, buf, routers[i]->next_hop);
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
num_routers = num_r;
router* routers[num_r];
int update = info.y;
char* iter = cntrl_payload + 4;
initialize_neighbors(num_r);
int temp = 0;
printf("------------------------------------------------\n");
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
	printf("ip of router %i: %u\n", c, ip);
	router* _router = malloc(sizeof(router));
	routers[c] = _router;
	routers[c]->id = id;
	routers[c]->port1 = port1;
	routers[c]->port2 = port2;
	routers[c]->cost = cost;
	routers[c]->ip = ip;
	printf("neighbors: ");
	if (cost == USHRT_MAX)routers[c]->next_hop = USHRT_MAX;
	else {
		neighbors[temp] = id;		
		printf("%u ", id);
		temp++;
		routers[c]->next_hop = id; 
	}
	printf("\n");
printf("------------------------------------------------\n");
}
get_ip();
build_adj_list(routers);
generate_response(sock_index);
};
