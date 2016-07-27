#include "cache.h"
#include "utils.h"
#include "header.h"
#include "flood.h"
#include "socket.h"
#include <string.h>

int ini_find_route(RREQ *rreq){
	//1. Check whether I'm the dst
	if(is_local_IP(rreq->dest_addr)){
		printf("You can't send message to yourself!\n");
		return -1;
	}

	//2. Find in the cache
	int path_len = find_path(rreq->dest_addr, rreq->addr);
	if(path_len != -1){
		printf("Find in the cache!\n");
		return 1;
	}

	//3. Broadcast
	rreq->request_id = get_unique_id(rreq->dest_addr);
	rreq->addr_num = 1;
	rreq->addr[0] = local_ip_num;
	char bc[sizeof(RREQ)+1];
	bc[0] = TYPE_RREQ;
	memcpy(bc+1, &rreq, sizeof(RREQ));
	send_broadcast(bc, strlen(bc));
	printf("No route found in cache, broadcast!\n");
	return 0;
}

int receive_RREQ(char *rreq){
	RREQ rc_rreq;
	memcpy(&rc_rreq, &rreq, sizeof(RREQ));
	if(check_new(rc_rreq.addr[0], rc_rreq.dest_addr, rc_rreq.request_id) == -1)
		return 0;

	int found = 0;

	//如果是dest
	if(rc_rreq.dest_addr == local_ip_num){
		rc_rreq.addr_num++;
		rc_rreq.addr[rc_rreq.addr_num-1] = local_ip_num;
		printf("I'm the destination, return RREP\n");
		found = 1;
	}
	//如果不是先查cache
	if(find_path(rc_rreq.dest_addr, rc_rreq.addr+rc_rreq.addr_num) != -1){
		printf("I find route in the cache, return RREP\n");
		found = 1;
	}

	if(found){
		RREP rrep;
		rrep.addr_num = rc_rreq.addr_num;
		rrep.dest_addr_index = rc_rreq.addr_num - 2;
		int i;
		for(i=0;i<rc_rreq.addr_num;++i)
			rrep.addr[i]=rc_rreq.addr[i];
		send_RREP(rrep);
		return 1;
	}

	//broadcast
	rc_rreq.addr[rc_rreq.addr_num] = local_ip_num;
	rc_rreq.addr_num++;
	char bc[sizeof(RREQ)+1];
	bc[0] = TYPE_RREQ;
	memcpy(bc+1, &rc_rreq, sizeof(RREQ));

	send_broadcast(bc, strlen(bc));
	printf("No route found in cache, broadcast!\n");
	return 0;
}

int send_RREP(RREP rrep){
	char sd[sizeof(RREP)+1];
	sd[0] = TYPE_RREP;
	memcpy(sd+1,&rrep,sizeof(RREP));
	send_unicast(rrep.addr[rrep.dest_addr_index],&sd,strlen(sd));
	printf("Send RREP successfully!\n");
}

int receive_RREP(char *rrep){
	RREP rc_rrep;
	memcpy(&rc_rrep, &rrep, sizeof(RREP));
	//如果自己是addr[0]	
	if(rc_rrep.dest_addr_index == 0){
		//加到cache
		insert_path(rc_rrep.addr[rc_rrep.addr_num-1],rc_rrep.addr_num,rc_rrep.addr);
		return 1;
	}
	//如果不是addr[0]
	rc_rrep.dest_addr_index--;
	send_RREP(rc_rrep);
	return 0;
}
