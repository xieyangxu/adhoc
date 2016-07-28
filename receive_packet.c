#include <stdio.h>
#include <string.h>
#include "cache.h"
#include "header.h"
#include "flood.h"
#include "utils.h"
#include "DSR.h"
int times = 0;
void receive_packet(char *packet, int packet_len){
	char data_type;	
	printf("#No.%d\n",++times);
	strncpy(&data_type,packet,1);

	char data[packet_len-1];
	memset(data, 0, sizeof(data));
	memcpy(data, packet+1, packet_len-1);

	//RREQ rreq;
	//memcpy(&rreq,&data,sizeof(RREQ));
	//printf("destis: %d\n",rreq.dest_addr);

    printf("The length of packet received is: %d\n",packet_len-1);
	//printf("the cotent of packet received is: %s\n",packet);
	
	
	switch(data_type){
		case TYPE_DATA: printf("The cotent of packet received is: %s\n",data);break;
		case TYPE_RREQ: printf("Type of packet received is: %s\nRun receive_RREQ\n","RREQ");receive_RREQ(data);break;
		case TYPE_RREP: printf("Type of packet received is: %s\nRun receive_RREP\n","RREP");receive_RREP(data);break;
	}
	
    
    //Todo

}



