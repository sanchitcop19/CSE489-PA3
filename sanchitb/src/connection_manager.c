/**
 * @connection_manager
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
 * Connection Manager listens for incoming connections/messages from the
 * controller and other routers and calls the desginated handlers.
 */

#include <sys/select.h>
#include <time.h>
#include <limits.h>
#include "../include/connection_manager.h"
#include "../include/global.h"
#include "../include/control_handler.h"
#include "../include/network_util.h"
#include "../include/map.h"

fd_set master_list, watch_list;
int head_fd;

void main_loop()
{
    int selret, sock_index, fdaccept;
	int router_disabled = 1;
	
	timeout.tv_sec = 1000;
    while(TRUE){
        watch_list = master_list;
	if (router_socket > -1 && router_disabled){
		FD_SET(router_socket, &master_list);
		router_disabled = 0;
			if (router_socket > head_fd)head_fd = router_socket;
		}
        selret = select(head_fd+1, &watch_list, NULL, NULL, &timeout);
time_t t = time(NULL);
printf("current time: %u\n", t);
printf("timeout: %u, %u\n", timeout.tv_sec, timeout.tv_usec);

        if(selret < 0)
            ERROR("select failed.");
	if (selret == 0){
		/*
		printq();
		printf("----------TIMEOUT BEGIN--------\n"); 
		timeout_qpair *front;
		timeout_qpair *next;
		printf("-----------------------------\n");
		printf("status of queue before popping: \n");
		printq();
		printf("head: %i\n", getfront());
		printf("tail: %i\n", getrear());
		front = (peek());
		pop();
		printf("-----------------------------\n");
		printf("status of queue after popping: \n");
		printq();
		printf("head: %i\n", getfront());
		printf("tail: %i\n", getrear());
		*/	time_t now = time(NULL);	
			printf("current time: %u\n", now);
			printf("sending updates...\n");
		

			/*
			printf("sending updates...\n");
			send_updates();
			printf("-----------------------------\n");
			printf("status of queue before pushing: \n");
		printf("head: %i\n", getfront());
		printf("tail: %i\n", getrear());
			printq();
			(front->to)->tv_sec = (now + update_interval);
			(front->to)->tv_usec = 0;
			push(front);
			printf("-----------------------------\n");
			printf("status of queue after pushing: \n");
			printq();
		printf("head: %i\n", getfront());
		printf("tail: %i\n", getrear());
			next = (peek());
			uint32_t save = (next->to)->tv_sec;*/
			unsigned int index = 0;
			router* rout;
			for (int z = 0; z < _numneighbors; ++z){
				rout = routers[z];
				if (rout->ip == self_ip)continue;
				if (now-(rout->lastupdate) > update_interval)rout->strike++;		
				const char *key;
				map_iter_t iter = map_iter(&weight_map);
				 while ((key = map_next(&weight_map, &iter))) {
					if (*map_get(&ip_map, key) == rout->ip){
						printf("ip_map for key: %s is %u\n", key, *map_get(&ip_map, key));
						printf("rout->ip: %u\n", rout->ip);
			                	index = *map_get(&index_map, key);
						break;
					}
					
				}
				if (rout->strike == 3){
					printf("removing router: %s\n", key);	
					dv[_row][index] = USHRT_MAX;
					uint16_t rid = rout->id;
					char sid[50];
					memset(sid, '\0', sizeof(sid));
					sprintf(sid, "%u", rid);
					map_set(&weight_map, sid, USHRT_MAX);
					map_set(&next_hop, sid, USHRT_MAX);
					
				}
			}
				
			send_updates();
			print_dv();
			timeout.tv_sec = update_interval;
			timeout.tv_usec = 0;
			printf("----------TIMEOUT END--------\n"); 
		/*else{
			(front->r)->strike++;
			(front->to)->tv_sec = (now + update_interval);
			(front->to)->tv_usec = 0;
			if (size() > 0){
				next = (peek());
				timeout.tv_sec = (next->to)->tv_sec - now;
				printf("editing the new front's timeout to %u\n", timeout.tv_sec);
				timeout.tv_usec = (next->to)->tv_usec;
			}	
			else{
				timeout.tv_sec = update_interval;
				printf("only one item, editing the select timeout to %u\n", timeout.tv_sec);
				timeout.tv_usec = 0;
			} 
			printf("-----------------------------\n");
			printf("status of queue before pushing: \n");
		printf("head: %i\n", getfront());
		printf("tail: %i\n", getrear());
			//printq();
			//push(front);
			printf("-----------------------------\n");
			printf("status of queue after pushing: \n");
		printf("head: %i\n", getfront());
		printf("tail: %i\n", getrear());
			//printq();
			
		}*/
	}
        /* Loop through file descriptors to check which ones are ready */
        for(sock_index=0; sock_index<=head_fd; sock_index+=1){
            if(FD_ISSET(sock_index, &watch_list)){

                /* control_socket */
                if(sock_index == control_socket){
                    fdaccept = new_control_conn(sock_index);

                    /* Add to watched socket list */
                    FD_SET(fdaccept, &master_list);
                    if(fdaccept > head_fd) head_fd = fdaccept;
                }

                /* router_socket */
                else if(sock_index == router_socket){
                    //call handler that will call recvfrom() .....
                    	uint32_t src_ip = 0;
			char* data = get_routing_update(sock_index, &src_ip);        
			printf("-------------------------------\n");
			printf("data received on router socket: %s\n", data);
			process_dv(data);
			bellman_ford();
			//printf("before receiving update: \n");
			//printq();
		/*	
			for (int z = 0; z < _numr; ++z){
				if (routers[z]->ip == src_ip){ 
					//timeout_qpair *qpair = malloc(sizeof(timeout_qpair));
					timeout_qpair *next = malloc(sizeof(timeout_qpair));
					qpair->r = routers[z];
					//TODO: change this to the actual timeout
					time_t now = time(NULL);
					struct timeval* tv = malloc(sizeof(struct timeval)); 
					tv->tv_sec = now+update_interval;
					tv->tv_usec = 0;
					qpair->to = tv;
					//push(qpair);		
					printf("current time: %u ,", now);
					printf("pushing router id: %u with timeout %u\n", routers[z]->id, tv->tv_sec);
					next = (peek()); 
					if ((next->r)->ip == src_ip && size() > 1){
						pop();
						next = (peek()); 
					}
					timeout.tv_sec = ((next->to)->tv_sec) - now;
					timeout.tv_usec = (next->to)->tv_usec;
				//verify the logic for this
					//remove if negative
			printf("after receiving update: \n");
			//printq();
			printf("-------------------------------\n");
				}	
			}*/
		}

                /* data_socket */
                else if(sock_index == data_socket){
                }

                /* Existing connection */
                else{
                    if(isControl(sock_index)){
                        if(!control_recv_hook(sock_index)) FD_CLR(sock_index, &master_list);
		    	else {
				printf("Received control message\n");
				//printq();
                    	}
			}
                    //else if isData(sock_index);
                    else ERROR("Unknown socket index");
                }
            }
		

        }
    }
}

void init()
{
    control_socket = create_control_sock();

    //router_socket and data_socket will be initialized after INIT from controller
	router_socket = -1;
	data_socket = -1;
    FD_ZERO(&master_list);
    FD_ZERO(&watch_list);

    /* Register the control socket */
    FD_SET(control_socket, &master_list);
    head_fd = control_socket;
    main_loop();
}
