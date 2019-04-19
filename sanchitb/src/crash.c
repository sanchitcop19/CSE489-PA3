#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
void crash(int sock_index){
	uint16_t response_len;
	response_len = CNTRL_RESP_HEADER_SIZE;

	char* header;
	header = create_response_header(sock_index, 4, 0, 0);
	
	sendALL(sock_index, header, response_len);

	free(header);
}
