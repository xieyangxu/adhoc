#include <stdio.h>
#include "send_packet.h"
#include "utils.h"
#include "header.h"
#include "flood.h"
#include "DSR.h"
#include <unistd.h>

#define RETRY_LIMIT 3

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
	for(i=0;i< RETRY_LIMIT;i++){
		if(ini_find_route(&rreq) == 1){//ini_find_route will return 1 if find a route successfully
			DATA data_packet;
			
			data_packet.addr_num = find_path(ip, data_packet.addr);//fill DATA.addr_num & DATA.addr[]
			data_packet.data_len = data_len;//fill DATA.data_len
			memcpy(data_packet.data, data, data_len);//fill DATA.data[]	

			char[sizeof(DATA) + 1] packet_content;
			packet_content[0] = TYPE_DATA;//add packet TYPE
			memcpy(packet_content + 1, &data_packet, sizeof(DATA));//transform into string format

			status = send_unicast(ip, packet_content, sizeof(DATA) + 1);//call send function

			printf("Find a way & send successfully!\n");
			break;
		}
		usleep(50000);

	}
	printf("Can find the way, send fail!\n");
	status = 0;
    return status;
}
