#include <stdio.h>

//brief: transfer the ip format from Dotted Decimal Notation to binary
//return: the binary format IP address
unsigned int ip_to_num(char* _ip);

//brief: check if the ip is the local IP address
//return: 1 if ip is the local IP
//        0 otherwise 
int is_local_IP(unsigned int ip);

//brief: send unicast to a specific ip
//return: the number of bytes that has been sent
//        -1 means failed
int send_unicast(unsigned int ip32, char *packet,int packet_len);

//brief: send broadcast to all the neighbors
//return: the number of bytes that has been sent
//        -1 means failed
int send_broadcast(char *packet,int packet_len);

