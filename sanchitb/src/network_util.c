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

