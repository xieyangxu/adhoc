#include <stdio.h>

#ifndef __socket__
#define __socket__

#define MAXBUF 65536
#define IP_LENGTH 20
#define BROADCAST_IP  "123.123.123.255"

char local_ip[IP_LENGTH+1];
unsigned int local_ip_num;
#define LOCAL_IP *local_ip
#define LOCAL_IP_NUM local_ip_num

//brief: using the UDP socket to send data between different computers
//       if you want to send a packet, use send_unicast in utils.h instead of this
int send_socket_packet(char * packet,int packet_len);


#endif
