/**
 * @network_util
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
 * Network I/O utility functions. send/recvALL are simple wrappers for
 * the underlying send() and recv() system calls to ensure nbytes are always
 * sent/received.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include<string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/global.h"

char* make_packet(){
	printf("making dv update packet----------------------\n");
	char* payload;
	//change ti nymber of neighbors
	uint16_t len = 8 + (_numr*12);
	printf("length of dv update packet: %u\n", len);
	payload = malloc(len);
	memset(payload, '\0', sizeof(payload));

	uint16_t numrouters = htons(_numr);
	char sid[50];
	memset(sid, '\0', sizeof(sid));
	sprintf(sid, "%u", self_id);
	printf("id in string: %s\n", sid);
	uint16_t routerport = htons(*map_get(&port_router_map, sid));
	printf("routerport(after htons): %u, id: %s\n", routerport, sid);
	const char *key;
        map_iter_t iter = map_iter(&weight_map);

	//adding first two fields
	memcpy(payload, &numrouters, sizeof(numrouters));
	memcpy(payload + TWOB, &routerport, sizeof(routerport));
	uint32_t ipcopy = htonl(self_ip);
	memcpy(payload + FOURB, &(ipcopy), sizeof(self_ip));
		
	short off = 0;
        while ((key = map_next(&weight_map, &iter))) {
                printf("%s -> %d\n", key, *map_get(&weight_map, key));
		printf("(adding info in dv update packet\n)");
		uint32_t ip_addr = *(map_get(&ip_map, key));
		ip_addr = htonl(ip_addr);
		uint16_t rport = *(map_get(&port_router_map, key));
		rport = htons(rport);
		uint16_t _id = strtol(key, NULL, 10);
		_id = htons(_id);	
		//should i choose base 10?
		uint16_t cost = *(map_get(&weight_map, key));
		cost = htons(cost);
		memcpy(payload + 0x08 + off, &ip_addr, sizeof(ip_addr));	
		memcpy(payload + 0x08 + off + FOURB, &rport, sizeof(rport));	
		memcpy(payload + 0x08 + off + FOURB + FOURB, &_id, sizeof(_id));	
		memcpy(payload + 0x08 + off + FOURB + FOURB + TWOB, &cost, sizeof(cost));	
		off += 0x0c;
        }
	return payload;

	
}
void send_updates(){
	char* packet = make_packet();
	char id_s[50];
	char ip_s[16];
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in to;
	printf("filling in the address\n");
	to.sin_family = AF_INET;
	//to.sin_port = htons(router_port);
	for (int z = 0; z < _numneighbors; ++z){
		memset(id_s, '\0', sizeof(id_s));
		unsigned long _id = neighbors[z];
		if (_id == self_id)continue;
		printf("printing id to array\n");
		sprintf(id_s, "%u", _id);	
		uint32_t ipaddr = *map_get(&ip_map, id_s);
		uint16_t port = *(map_get(&port_router_map, id_s));
		to.sin_port = htons(port);
		to.sin_addr.s_addr = htonl(ipaddr);
		sendto(sock, packet, 68, 0, &to, sizeof(to));
	}	
	printf("reached the end\n");
}
char* get_routing_update(int sock_index, uint32_t* src_ip){
	char* data = malloc(69*(sizeof(char)));
        struct sockaddr_in from;
	memset(data, '\0', 69);
	int len = sizeof(from);
        int bytes = recvfrom(sock_index, data, 68, 0, &from, &len);
	*src_ip = ntohl(from.sin_addr.s_addr);
	return data;

}
ssize_t recvALL(int sock_index, char *buffer, ssize_t nbytes)
{
    ssize_t bytes = 0;
    bytes = recv(sock_index, buffer, nbytes, 0);

    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += recv(sock_index, buffer+bytes, nbytes-bytes, 0);
	printf("bufferlength: %i\n", strlen(buffer));
	printf("buffer: %u\n", (unsigned char)buffer);
    return bytes;
}

ssize_t sendALL(int sock_index, char *buffer, ssize_t nbytes)
{
    ssize_t bytes = 0;
    bytes = send(sock_index, buffer, nbytes, 0);

    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += send(sock_index, buffer+bytes, nbytes-bytes, 0);

    return bytes;
}

char* ip_from_long(unsigned long ip){
	struct in_addr address = {ntohl(ip)};
	char* result = inet_ntoa(address);
	printf(" receiver ip: %s\n", result);
	return result;
}

void get_ip(){
        int dummy_socket;
        struct sockaddr_in addr;
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        int len = sizeof addr;
        getaddrinfo("www.beej.us", "80", &hints, &res);

        dummy_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

         if (connect(dummy_socket, res->ai_addr, res->ai_addrlen) < 0){
                perror("connect failed\n");
        }

         if (getsockname(dummy_socket,(struct sockaddr*) &addr,(socklen_t*)(&len))  < 0){
                perror("getsockname failed\n");
        }

        if (inet_ntop(AF_INET, &(addr.sin_addr), ip, sizeof ip) == NULL){
                perror("inettnop returned null\n");
        }
	ip_l = ntohl(addr.sin_addr.s_addr);
	printf("IP: %u\n", ntohl(addr.sin_addr.s_addr));
        printf("IP:%s\n", ip);


}

