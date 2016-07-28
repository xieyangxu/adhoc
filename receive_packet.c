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
	printf("#No.%d####################################################\n",++times);
	strncpy(&data_type,packet,1);

	char data[packet_len-1];
	memset(data, 0, sizeof(data));
	memcpy(data, packet+1, packet_len-1);

	//RREQ rreq;
	//memcpy(&rreq,&data,sizeof(RREQ));
	//printf("destis: %d\n",rreq.dest_addr);

    printf("#\tThe length of packet received is: %d\t\t#\n",packet_len-1);
	//printf("the cotent of packet received is: %s\n",packet);
	
	
	switch(data_type){
		//case TYPE_DATA: printf("The cotent of packet received is: %s\n",data);break;
		case TYPE_DATA: printf("#\tType of packet received is: %s\t\t#\n","DATA");print_packet(data);break;
		case TYPE_RREQ: printf("#\tType of packet received is: %s\t\t#\n#\t\t\t\t\t\t\t#\n#\t\tRun receive_RREQ\t\t\t#\n","RREQ");receive_RREQ(data);break;
		case TYPE_RREP: printf("#\tType of packet received is: %s\t\t#\n#\t\t\t\t\t\t\t#\n#\t\tRun receive_RREP\t\t\t#\n","RREP");receive_RREP(data);break;
	}
	
    
    //Todo

}

void print_packet(char* packet_content)
{
	DATA rc_data;
	memcpy(&rc_data, packet_content, sizeof(DATA));	
	printf("#################Receive a DATA packet!##################\n\tThe cotent of packet received is:\t\t\n\t%s\n#########################################################\n", rc_data.data);
	return;
}


