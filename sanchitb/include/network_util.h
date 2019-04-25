#ifndef NETWORK_UTIL_H_
#define NETWORK_UTIL_H_

ssize_t recvALL(int sock_index, char *buffer, ssize_t nbytes);
ssize_t sendALL(int sock_index, char *buffer, ssize_t nbytes);
char* ip_from_long(unsigned long);
void get_ip();
char* get_routing_update(int, uint32_t*);
void send_updates();
#endif
