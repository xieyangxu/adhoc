#include "cache.h"
#include "utils.h"
#include "header.h"
#include "flood.h"
#include "socket.h"
#include <string.h>

int ini_find_route(RREQ *rreq){

	char dest_ip[20];
	num_to_ip(rreq->dest_addr, dest_ip);
	printf("\n\n\nInitialize finding a route to %s\n",dest_ip);

	//1. Check whether I'm the dst
	if(is_local_IP(rreq->dest_addr)){
		printf("You can't send message to yourself!\n\n");
		return -1;
	}

	//2. Find in the cache
	int path_len = find_path(rreq->dest_addr, rreq->addr);
	if(path_len != -1){
		printf("Ini_find a route in the cache!\n\n");
		return 1;
	}

	//3. Broadcast
	rreq->request_id = get_unique_id(rreq->dest_addr);
	rreq->addr_num = 1;
	rreq->addr[0] = local_ip_num;
	char bc[sizeof(RREQ)+1];
	bc[0] = TYPE_RREQ;
	memcpy(bc+1, rreq, sizeof(RREQ));
	check_new(rreq->addr[0], rreq->dest_addr, rreq->request_id);
	send_broadcast(bc, sizeof(RREQ)+1);
	printf("No route found in cache, ini_broadcast!\n\n");
	return 0;
}

int receive_RREQ(char *rreq){
	
	RREQ rc_rreq;
	memcpy(&rc_rreq, rreq, sizeof(RREQ));
	//myself
	if(rc_rreq.addr[0] == local_ip_num){
		printf("#\tIt's my packet, ignore it.\t\t\t#\n#########################################################\n");
		return 0;
	}
	char rreq_ip[20];
	char rreq_dest_ip[20];
	char rreq_sour_ip[20];
	num_to_ip(rc_rreq.addr[rc_rreq.addr_num-1], rreq_ip);
	num_to_ip(rc_rreq.dest_addr, rreq_dest_ip);
	num_to_ip(rc_rreq.addr[0], rreq_sour_ip);
	printf("#\tReceive a RREQ from:\t%s\t\t#\n#\tThe source is:\t\t%s\t\t#\n#\tAnd he wants to find:\t%s\t\t#\n", rreq_ip, rreq_sour_ip, rreq_dest_ip);
	//printf("srs:%d,%d\n",rc_rreq.addr[0], rc_rreq.dest_addr);

	if(check_new(rc_rreq.addr[0], rc_rreq.dest_addr, rc_rreq.request_id) == -1){
		printf("#\t\t\tCheck already!\t\t\t#\n#########################################################\n");
		return 0;
	}
	int found = 0;

	//printf("RREQ:%d\nLocal:%d\n",rc_rreq.dest_addr,local_ip_num);
	rc_rreq.addr_num++;
	rc_rreq.addr[rc_rreq.addr_num-1] = local_ip_num;
	char rreq_reIP[20];
	num_to_ip(rc_rreq.addr[rc_rreq.addr_num-2], rreq_reIP);
	//如果是dest
	if(rc_rreq.dest_addr == local_ip_num){	
		printf("#\tI'm dest, send RREP to:\t%s\t\t#\n", rreq_reIP);
		found = 1;
	}
	//如果不是先查cache
	int find_len = find_path(rc_rreq.dest_addr, rc_rreq.addr+rc_rreq.addr_num-1);
	if(find_len != -1){
		rc_rreq.addr_num += find_len-1;
		printf("#\tFind in cache, return RREP!!!\t\t\t#\n");
		found = 2;
	}

	if(found){
		RREP rrep;
		rrep.addr_num = rc_rreq.addr_num;
		rrep.dest_addr_index = rc_rreq.addr_num - 2 - (found-1)*(find_len-1);
		int i;
		for(i=0;i<rc_rreq.addr_num;++i)
			rrep.addr[i]=rc_rreq.addr[i];
		send_RREP(rrep);
		return 1;
	}

	//broadcast
	char add_ip[20];
	num_to_ip(local_ip_num, add_ip);
	printf("#\t\t\tAdd me: %s\t\t#\n",add_ip);
	check_new(rc_rreq.addr[0], rc_rreq.dest_addr, rc_rreq.request_id);
	//rc_rreq.addr[rc_rreq.addr_num] = local_ip_num;
	//rc_rreq.addr_num++;
	char bc[sizeof(RREQ)+1];
	bc[0] = TYPE_RREQ;
	memcpy(bc+1, &rc_rreq, sizeof(RREQ));

	send_broadcast(bc, sizeof(RREQ)+1);
	printf("#\t\tBroadcast!\t\t\t\t#\n#########################################################\n");
	return 0;
}

int send_RREP(RREP rrep){
	char sd[sizeof(RREP)+1];
	char tar_ip[20];
	num_to_ip(rrep.addr[rrep.dest_addr_index], tar_ip);
	printf("#\tThe target of RREP is:\t%s\t\t#\n", tar_ip);
	sd[0] = TYPE_RREP;
	memcpy(sd+1,&rrep,sizeof(RREP));
	send_unicast(rrep.addr[rrep.dest_addr_index],&sd,sizeof(RREP) + 1);
	printf("#\t\tSend RREP successfully!\t\t\t#\n#########################################################\n");
}

int receive_RREP(char *rrep){
	RREP rc_rrep;
	memcpy(&rc_rrep, rrep, sizeof(RREP));

	char rrep_from[20];
	char rrep_sour[20];
	num_to_ip(rc_rrep.addr[0], rrep_sour);
	num_to_ip(rc_rrep.addr[rc_rrep.dest_addr_index+1], rrep_from);
	printf("#\tI get a RREP from:\t%s\t\t#\n#\tAnd the source is:\t%s\t\t#\n", rrep_from, rrep_sour);
	
	//如果自己是addr[0]	
	if(rc_rrep.dest_addr_index == 0){
		//加到cache
		printf("#\tI'm the source, and insert to the cache!\t#\n#########################################################\n");
		insert_path(rc_rrep.addr[rc_rrep.addr_num-1],rc_rrep.addr_num,rc_rrep.addr);
		return 1;
	}
	//如果不是addr[0]

	printf("#\tI'm not the source, send continue!\t\t#\n#\tAnd I will insert the dest into my cache.\t#\n#########################################################\n");
	insert_path(rc_rrep.addr[rc_rrep.addr_num-1], rc_rrep.addr_num-rc_rrep.dest_addr_index, rc_rrep.addr+rc_rrep.dest_addr_index);
	rc_rrep.dest_addr_index--;
	send_RREP(rc_rrep);
	return 0;
}
