#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "map.h"
typedef enum {FALSE, TRUE} bool;
#define ERROR(err_msg) {perror(err_msg); exit(EXIT_FAILURE);}
#define FOURB 0x04
#define TWOB 0x02
#define BYTE 0x01
/* https://scaryreasoner.wordpress.com/2009/02/28/checking-sizeof-at-compile-time/ */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)])) // Interesting stuff to read if you are interested to know how this works
unsigned long neighbors[5];
uint16_t self_id;
unsigned int _row;
unsigned long self_ip;
uint16_t router_port; 
uint16_t data_port;

int router_socket, data_socket;

uint16_t CONTROL_PORT;
uint16_t DATA_PORT;
uint16_t ROUTER_PORT;
uint_map_t weight_map;
uint_map_t ip_map;
uint_map_t port_router_map;
uint_map_t port_data_map;
uint_map_t next_hop;
uint_map_t index_map;
uint_map_t sock_map;

char ip[16];
unsigned long ip_l;
int dv[5][5]; 
int update_interval;

int _numneighbors;
int _numr;
struct timeval timeout;


typedef struct{
        uint16_t id;
        uint16_t port1;
        uint16_t port2;
        uint16_t cost;
        uint32_t ip;
        uint16_t next_hop;
	short strike;
	unsigned long lastupdate;
} router;

typedef struct {
	char ip[16];
	uint8_t ttl;
	uint8_t transfer_id;
	uint16_t sequence_num;
	char* filename;
	
} receiver;
router** routers;
typedef struct{
	long x, y;
}pair;

typedef struct{
	router* r;
	struct timeval * to;
}timeout_qpair;

timeout_qpair* queue[5];


pair get_info(int, char*, int, int, int);
receiver get_sendfile_info(char*);
char* char2bits(char);
void initialize_neighbors(int);
void print_dv();
void print_map(uint_map_t);
#endif
void process_dv();
