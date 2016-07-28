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
		case TYPE_DATA: printf("#\tType of packet received is: %s\t\t#\n","DATA");on_receive_DATA(data);break;
		case TYPE_RREQ: printf("#\tType of packet received is: %s\t\t#\n#\t\t\t\t\t\t\t#\n#\t\tRun receive_RREQ\t\t\t#\n","RREQ");receive_RREQ(data);break;
		case TYPE_RREP: printf("#\tType of packet received is: %s\t\t#\n#\t\t\t\t\t\t\t#\n#\t\tRun receive_RREP\t\t\t#\n","RREP");receive_RREP(data);break;
	}
	
    
    //Todo

}
int on_receive_DATA(char* data)
{
	int status = 0;
	//status
	//1 = is D, success
	//-1 = is S, Loop, discard
	//2 = is Diliver, success
	//0 = error
	DATA packet_DATA;
	memcpy(&packet_DATA, data, sizeof(DATA));//trans to DATA format

	if(is_local_IP(packet_DATA.addr[packet_DATA.addr_num - 1]))//if it is destination itself
	{
		print_DATA(packet_DATA);
		return 1;
	}
	else if(is_local_IP(packet_DATA.addr[0]))//if it is source it self
	{
		printf("Receive a DATA packet!!!\nLoop, discard\n");
		return -1;
	}
	else{
		int i;
		for(i=1; i < (packet_DATA.addr_num - 1); i++)
		{
			if(is_local_IP(packet_DATA.addr[i]))//it is a deliver
			{
				char sd_packet[sizeof(DATA) + 1];//trans to char* format
				sd_packet[0] = TYPE_DATA;
				memcpy(sd_packet + 1, &packet_DATA, sizeof(DATA));
				send_unicast(packet_DATA.addr[i - 1], sd_packet, sizeof(DATA) + 1);//deliver to the next
				printf("Receive a DATA packet!!!\nDeliver it successfully!\n");
				break;
			}
		}
		return 2;
	}
	return 0;
}

void print_DATA(DATA* received_packet_data)
{
	//DATA rc_data;
	//memcpy(&rc_data, received_packet_data, sizeof(DATA));	
	printf("#################Receive a DATA packet!##################\n\tThe cotent of packet received is:\t\t\n\t%s\n#########################################################\n", received_packet_data->data);
}



