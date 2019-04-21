#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/map.h"

void send_packet(char* content, int sock_index, char* payload){
	char* ip_b = strcat(char2bits(payload[0]), char2bits(payload[1]));
        ip_b = strcat(ip_b, char2bits(payload[2]));	
        ip_b = strcat(ip_b, char2bits(payload[3]));
	char * ip = ip_from_long(strtol(ip_b, NULL, 2));
}

void packetize(char name[], int sock_index, char* payload){
	FILE * file;
	int size;
	file = fopen(name, "rb");
	fseek (file , 0 , SEEK_END);
	size = ftell (file);
	rewind(file);
	char* content = malloc(size);
	memset(content, '\0', size);
	printf("filesize: %i\n", size);
	int n = 0;
	int location = ftell(file);
	while (location < size){
		n = fread(content+location, 1, 1024, file );	
		send_packet(content+location, sock_index, payload);
		location = ftell(file);
		printf("loc: %i", location);
	}
	fclose(file);
	FILE* fileo;
	fileo = fopen("testfile1-o", "wb");
	fwrite(content, 1, location, fileo) ;
	fclose(fileo);
	
}
void send_file(int sock_index, char* payload){
	uint16_t payload_len, response_len;
	char* header;

	payload_len = 0;
	header = create_response_header(sock_index, 5, 0, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE + payload_len;;
	sendALL(sock_index, header, response_len);
	free(header);
		
	packetize("testfile1", sock_index, payload);
}
