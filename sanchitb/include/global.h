#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "map.h"
typedef enum {FALSE, TRUE} bool;
#define ERROR(err_msg) {perror(err_msg); exit(EXIT_FAILURE);}

/* https://scaryreasoner.wordpress.com/2009/02/28/checking-sizeof-at-compile-time/ */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)])) // Interesting stuff to read if you are interested to know how this works

uint16_t CONTROL_PORT;
uint_map_t weight_map;
uint_map_t ip_map;
uint_map_t port_router_map;
uint_map_t port_data_map;
uint_map_t next_hop;

typedef struct{
        uint16_t id;
        uint16_t port1;
        uint16_t port2;
        uint16_t cost;
        uint32_t ip;
        uint16_t next_hop;
} router;

typedef struct{
	long x, y;
}pair;

pair get_info(int, char*, int, int, int);

#endif
