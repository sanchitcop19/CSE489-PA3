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

int num_routers = 0;
typedef map_t(unsigned int) uint_map_t;
typedef struct{
	uint16_t id;
	uint16_t port1;
	uint16_t port2;
	uint16_t cost;
	uint32_t ip;
} router;
void convert_payload(){

}
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
	uint_map_t weight_map;
	uint_map_t ip_map;
	uint_map_t port_router_map;
	uint_map_t port_data_map;
	char buf[2];

	map_init(&weight_map);
	map_init(&ip_map);	
	map_init(&port_router_map);	
	map_init(&port_data_map);	

	for (int i = 0; i < num_routers; ++i){

		memset(buf, '\0', sizeof buf);
		sprintf(buf, "%u", routers[i]->id);

		map_set(&weight_map, buf, routers[i]->cost);
		
		map_set(&ip_map, buf, routers[i]->ip);
		
		map_set(&port_router_map, buf, routers[i]->port1);
		
		map_set(&port_data_map, buf, routers[i]->port2);
	}
	const char *key;
map_iter_t iter = map_iter(&m);

while ((key = map_next(&weight_map, &iter))) {
  printf("%s -> %u\n", key, *map_get(&weight_map, key));
}

} 

void init_response(int sock_index, char* cntrl_payload){
printf("initializing based off of: %s\n", cntrl_payload);	
//Assuming self to be euston
//Save routers at the id'th position in the array
num_routers = 5;
uint16_t interval = 3;
uint16_t id1 = 1;
uint16_t port11 = 3452;
uint16_t port12 = 2344;
uint16_t cost1 = 0;
uint32_t ip1 = 2160927778;
router* router1 = malloc(sizeof(router));
router1->id = id1;
router1->port1 = port11;
router1->port2 = port12;
router1->cost = cost1;
router1->ip = ip1;

uint16_t id2 = 2;
uint16_t port21 = 4562;
uint16_t port22 = 2345;
uint16_t cost2 = 7;
uint32_t ip2 = 2160927790;
router*router2= malloc(sizeof(router));
router2->id = id2;
router2->port1 = port21;
router2->port2 = port22;
router2->cost = cost2;
router2->ip = ip2;

uint16_t id3 = 3;
uint16_t port31 = 8356;
uint16_t port32 = 1635;
uint16_t cost3= USHRT_MAX;
uint32_t ip3 = 2160927777;
router*router3= malloc(sizeof(router));
router3->id = id3;
router3->port1 = port31;
router3->port2 = port32;
router3->cost = cost3;
router3->ip = ip3;

uint16_t id4 = 4;
uint16_t port41 = 4573;
uint16_t port42 = 1678;
uint16_t cost4 = 2;
uint32_t ip4 = 2160927779;
router*router4= malloc(sizeof(router));
router4->id = id4;
router4->port1 = port41;
router4->port2 = port42;
router4->cost = cost4;
router4->ip = ip4;

uint16_t id5 = 5;
uint16_t port51 = 3456;
uint16_t port52 = 1946;
uint16_t cost5 = USHRT_MAX;
uint32_t ip5 = 2160927780;
router*router5= malloc(sizeof(router));
router5->id = id5;
router5->port1 = port51;
router5->port2 = port52;
router5->cost = cost5;
router5->ip = ip5;
router* routers[5] = {
	router1,
	router2,
	router3,
	router4,
	router5
};
build_adj_list(routers);
generate_response(sock_index);
};
