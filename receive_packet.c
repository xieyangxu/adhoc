#include <stdio.h>
#include <string.h>
#include "cache.h"
#include "header.h"
#include "flood.h"
#include "utils.h"
#include "DSR.h"
#include "socket.h"
int times = 0;

extern int already_send_packet;
extern int get_ACK;
extern DATA* local_DATA;

void receive_packet(char *packet, int packet_len){
	char data_type;	
	printf("#No.%d\t#################################################\n",++times);
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
		case TYPE_BRK: printf("#\tType of packet received is: %s\t\t\t#\n#\t\t\t\t\t\t\t#\n#\t\tRun receive_BRK\t\t\t\t#\n","BRK");on_receive_BRK(data);break;
		case TYPE_ACK: printf("#\tType of packet received is: %s\t\t\t#\n#\t\t\t\t\t\t\t#\n#\t\tRun receive_ACK\t\t\t\t#\n","ACK");on_receive_ACK(data);break;
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
	memcpy(local_DATA, data, sizeof(DATA));

	if(is_local_IP(packet_DATA.addr[packet_DATA.addr_num - 1]))//if it is destination itself
	{
		// send ACK to backward
		char sd_ACK[sizeof(ACK) + 1];
		sd_ACK[0] = TYPE_ACK;
		ACK ack;
		ack.dest_addr = local_ip_num;//packet_DATA.addr[packet_DATA.addr_num - 1];
		ack.src_addr = packet_DATA.addr[0];
		memcpy(sd_ACK + 1, &ack, sizeof(ACK));
		send_unicast(packet_DATA.addr[packet_DATA.addr_num - 2], sd_ACK, sizeof(ACK) + 1);
		
		print_DATA(&packet_DATA);
		return 1;
	}
	else if(is_local_IP(packet_DATA.addr[0]))//if it is source it self
	{
		printf("#\tReceive a DATA packet!!!\t\t\t#\nLoop, discard\n");
		return -1;
	}
	else{
		int i;
		for(i=1; i < (packet_DATA.addr_num - 1); i++)
		{
			if(is_local_IP(packet_DATA.addr[i]))//it is a deliver
			{
				// send ACK to backward
				char sd_ACK[sizeof(ACK) + 1];
				sd_ACK[0] = TYPE_ACK;
				ACK ack;
				ack.dest_addr = packet_DATA.addr[packet_DATA.addr_num - 1];
				ack.src_addr = packet_DATA.addr[0];
				memcpy(sd_ACK + 1, &ack, sizeof(ACK));
				send_unicast(packet_DATA.addr[i - 1], sd_ACK, sizeof(ACK) + 1);
				
				// send pack_DATA to forward
				char sd_packet[sizeof(DATA) + 1];//trans to char* format
				sd_packet[0] = TYPE_DATA;
				memcpy(sd_packet + 1, &packet_DATA, sizeof(DATA));
				send_unicast(packet_DATA.addr[i + 1], sd_packet, sizeof(DATA) + 1);//deliver to the next
				printf("#\tReceive a DATA packet!!!\t\t\t#\n#\tDeliver it successfully!\t\t\t#\n#########################################################\n");
				
				// set already_send_packet to 1
				already_send_packet = 1; 
				
				break;
			}
		}
		return 2;
	}
	return 0;
}

int on_receive_BRK(char* data)
{
	int status = 0;
	//1 = delete successfully
	//2 = no such route
	//printf("#\tReceive a break!!!\t\t\t#\n");
	BRK brk;
	memcpy(&brk, data, sizeof(BRK));

	unsigned int addr[MAX_ADDR_NUM];
	if(find_path(brk.dest_addr, addr))//if path to the broken addr exits
	{
		delete_path(brk.dest_addr);
		
		status = 1;
		int i;
		for(i = 1; i < (brk.addr_num - 1); i++)
		{
			if(is_local_IP(brk.addr[i]))//if(is_local_IP(brk.dest_addr))
			{
				char sd_packet[sizeof(BRK) + 1];
				sd_packet[0] = TYPE_BRK;
				memcpy(sd_packet + 1, &brk, sizeof(BRK));
				send_unicast(brk.addr[i - 1], sd_packet, sizeof(BRK) + 1);
				break;
			}
		}
		
		// is source
		if(is_local_IP(brk.addr[0]))
		{
			RREQ rreq;
			rreq.dest_addr = brk.dest_addr;
			ini_find_route(&rreq);
			usleep(200000);
			char packet_content[sizeof(DATA) + 1];
			packet_content[0] = TYPE_DATA;//add packet TYPE

			local_DATA->addr_num = find_path(brk.dest_addr, local_DATA->addr);

			memcpy(packet_content + 1, local_DATA, sizeof(DATA));//transform into string format

			status = send_unicast(local_DATA->addr[1], packet_content, sizeof(DATA) + 1);//call send function
			
			// set already_send_packet to 1
			already_send_packet = 1;
			printf("Resend succefully!!!\n");

		}
	}
	else{
		status = 2;//no such path
	}
	
	return status;
}

int on_receive_ACK(char* data)
{
	ACK ack;
	memcpy(&ack, data, sizeof(ACK));
	char ack_ip[20];
	num_to_ip(ack.dest_addr,ack_ip);
	printf("#\tReceive ACK from:\t%s\t\t#\n",ack_ip);
	printf("#########################################################\n");
	// set get_ACK to 1
	get_ACK = 1;
	
	return 1;
}

void print_DATA(DATA* received_packet_data)
{
	char data[received_packet_data->data_len+1];
	memcpy(&data, received_packet_data->data, received_packet_data->data_len);
	data[received_packet_data->data_len]='\0';
	printf("#################Receive a DATA packet!##################\n\tThe cotent of packet received is:\t\t\n\t%s\n#########################################################\n", data);
}



