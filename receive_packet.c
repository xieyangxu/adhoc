#include <stdio.h>
#include <string.h>
#include "cache.h"
#include "header.h"
#include "flood.h"
#include "utils.h"
#include "DSR.h"

void receive_packet(char *packet, int packet_len){
	char data_type;
	strncpy(data_type,packet,1);
	char data[packet_len];
	memset(data, 0, sizeof(data));
	strncpy(data,packet+1,packet_len-1);

    printf("the length of packet received is: %d\n",packet_len-1);
	printf("the type of packet received is: %s",data_type);
	
	switch(data_type){
		case TYPE_DATA: printf("the cotent of packet received is: %s\n",data);
		case TYPE_RREQ: receive_RREQ(data);
	
	}
	
    
    //Todo

}



