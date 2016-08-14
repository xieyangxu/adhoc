#include <stdio.h>
#include "send_packet.h"
#include "utils.h"
#include "header.h"
#include "flood.h"
#include "DSR.h"
#include "cache.h"
#include <unistd.h>

#define RETRY_LIMIT 31
extern int already_send_packet;
extern DATA* local_DATA;

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
		int x = ini_find_route(&rreq);
		if( x== 1){//ini_find_route will return 1 if find a route successfully
			DATA data_packet;
			
			data_packet.addr_num = find_path(ip, data_packet.addr);//fill DATA.addr_num & DATA.addr[]
			data_packet.data_len = data_len;//fill DATA.data_len
			memcpy(data_packet.data, data, data_len);//fill DATA.data[]	
			memcpy(local_DATA, &data_packet, sizeof(DATA));

			char packet_content[sizeof(DATA) + 1];
			packet_content[0] = TYPE_DATA;//add packet TYPE
			memcpy(packet_content + 1, &data_packet, sizeof(DATA));//transform into string format

			status = send_unicast(data_packet.addr[1], packet_content, sizeof(DATA) + 1);//call send function
			
			// set already_send_packet to 1
			already_send_packet = 1;

			printf("Find a way & send successfully!\n");
			break;
		}
		else if(x == -1)return -1;
		usleep(50000);

	}
	//printf("Can find the way, send fail!\n");
	status = 0;
    return status;
}
