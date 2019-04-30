#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h" 
#include "../include/map.h"

void update(int sock_index, char* payload){
	/*parse router id and cost from payload*/	
	uint16_t payload_len, response_len;
	payload_len = 0;	
	char* header;
	header = create_response_header(sock_index, 3, 0, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE + payload_len;
	sendALL(sock_index, header, response_len);
	free(header);
	pair result = get_info(16, payload, 0, 3, 0);
	unsigned long id = result.x;
	unsigned long cost = result.y;
	char buf[2];memset(buf, '\0', sizeof buf);
	sprintf(buf, "%u", id);
	map_set(&weight_map, buf, cost);
	const char *key;
	map_iter_t iter = map_iter(&weight_map);
	
	dv[_row][(*map_get(&index_map, buf))] = cost;
	//TODO: is this right?
	dv[(*map_get(&index_map, buf))][_row]= cost;
	bellman_ford();	
	while ((key = map_next(&weight_map, &iter))) {
  		printf("%s -> %d\n", key, *map_get(&weight_map, key));
	}
}

