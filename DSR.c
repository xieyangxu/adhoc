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
	
	char *bc = (char *)&rreq;
	send_broadcast(bc, strlen(bc));
	printf("No route found in cache, broadcast!\n");
	return 0;
}

int receive_RREQ(char *rreq){
	RREQ rc_rreq;
	memcpy(&rc_rreq, &rreq, sizeof(RREQ));
	if(check_new(rc_rreq.addr[0], rc_rreq.dest_addr, rc_rreq.request_id) == -1)
		return 0;

	//如果是dest
	if(rreq->dest_addr == local_ip_num){
		rreq->addr_num++;
		rreq->addr[rreq->addr_num-1] = local_ip_num;
		printf("I'm the destination, return RREP\n");
	}
	//如果不是先查cache
	if(find_path(rreq->dest_addr, rreq->addr+rreq->addr_num) != -1){
		printf("I find route in the cache, return RREP\n");
		
	}

	//broadcast
	rreq->addr[rreq->addr_num] = local_ip_num;
	rreq->addr_num++;
	char *bc = (char *)&rreq;
	send_broadcast(bc, strlen(bc));
	printf("No route found in cache, broadcast!\n");
	return 0;
}
