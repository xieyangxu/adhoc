#include <stdio.h>
#include "send_packet.h"
#include "utils.h"
#include "header.h"
#include "flood.h"
#include "DSR.h"
#include <unistd.h>

int send_packet(unsigned int ip, char *data,int data_len){
    int status = 0;//send_unicast(ip, data, data_len);
    //Todo
	RREQ rreq;
	rreq.dest_addr = ip;

	//get the status:
	//-1: failed because you are the dest
	//1: get the route in the cache, rreq.addr
	//0: broadcast
	int i;
	for(i=0;i<3;i++){
		if(ini_find_route(&rreq) == 1){
			//send();
			printf("Find a way and send the packet!\n");
		}
		usleep(50000);

	}
	printf("Can find the way, send fail!\n");
	status = 0;
    return status;
}
