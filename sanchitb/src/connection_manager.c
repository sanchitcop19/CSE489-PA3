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

#include "../include/connection_manager.h"
#include "../include/global.h"
#include "../include/control_handler.h"
#include "../include/network_util.h"
#include "../include/Queue.h"

fd_set master_list, watch_list;
int head_fd;

void main_loop()
{
    int selret, sock_index, fdaccept;
	int router_disabled = 1;
    while(TRUE){
        watch_list = master_list;
	if (router_socket > -1 && router_disabled){
		FD_SET(router_socket, &master_list);
		router_disabled = 0;
			if (router_socket > head_fd)head_fd = router_socket;
		}
        selret = select(head_fd+1, &watch_list, NULL, NULL, NULL);

        if(selret < 0)
            ERROR("select failed.");

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
			printf("data received on router socket: %s\n", data);
			for (int z = 0; z < _numr; ++z){
				if (routers[z]->ip == src_ip){ 
					push(&queue, routers[z]);		
				}	
			}
		}

                /* data_socket */
                else if(sock_index == data_socket){
                }

                /* Existing connection */
                else{
                    if(isControl(sock_index)){
                        if(!control_recv_hook(sock_index)) FD_CLR(sock_index, &master_list);
		    	else printf("Received control message\n");
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
    qinit(&queue, sizeof(router)); 
    main_loop();
}
