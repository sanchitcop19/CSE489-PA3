#include "../include/global.h"

void routing_table(int sock_index){
	//Use this function assuming the routing table is accurate, only to send
	//Include the actual bellman ford implementation in globals
	const char *key;
	printf("here\n");
	map_iter_t iter = map_iter(&next_hop);

	while ((key = map_next(&next_hop, &iter))) {
  		printf("%s -> %u\n", key, *map_get(&next_hop, key));
	}
	
}
