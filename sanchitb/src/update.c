#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h" 
#include "../include/map.h"

void update(int sock_index, char* payload){
	/*parse router id and cost from payload*/	
	uint16_t cost = 2;
	uint16_t id = 2;	
	uint16_t payload_len, response_len;
	payload_len = 0;	
	char* header;
	header = create_response_header(sock_index, 3, 0, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE + payload_len;
	sendALL(sock_index, header, response_len);
	free(header);
	char buf[2];memset(buf, '\0', sizeof buf);
	sprintf(buf, "%u", id);
	map_set(&weight_map, buf, cost);
	const char *key;
	map_iter_t iter = map_iter(&weight_map);

	while ((key = map_next(&weight_map, &iter))) {
  		printf("%s -> %d\n", key, *map_get(&weight_map, key));
	}
}

