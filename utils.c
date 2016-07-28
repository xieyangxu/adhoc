#include "utils.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
/*
int send_unicast(char *ip, char *packet,int packet_len){
    char buffer[MAXBUF];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, ip);
    strcpy(buffer+IP_LENGTH, packet);
    int status = send_socket_packet(buffer, packet_len+IP_LENGTH);
    return status;
}*/

int send_unicast(unsigned int ip32, char *packet,int packet_len){
    char ip[20];
	char buffer[MAXBUF];
    memset(ip, 0, sizeof(ip));
	memset(buffer, 0, sizeof(buffer));
	num_to_ip(ip32,ip); 
	
    unsigned char *bytes = (unsigned char *) &ip32;  
    sprintf (ip, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
    printf("ip: %s\n",ip);
    strcpy(buffer, ip);
    memcpy(buffer+IP_LENGTH, packet, packet_len);
    int status = send_socket_packet(buffer, packet_len+IP_LENGTH);
    return status;
}

int send_broadcast(char *packet,int packet_len){
    char buffer[MAXBUF];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, BROADCAST_IP);
    memcpy(buffer+IP_LENGTH, packet,packet_len);
    int status = send_socket_packet(buffer, packet_len+IP_LENGTH);
    return status;
}

unsigned int ip_to_num(char* _ip){
    char ip[20];
    memset(ip,0,sizeof(ip));
    strncpy(ip, _ip,strlen(_ip));

    char* p = NULL;
    int sections[4];
    int i = 0;

    p = strtok(ip,".");
    while(p != NULL){
        sections[i] = atoi(p);
        p = strtok(NULL,".");
        ++i;
    }

    unsigned int num = 0;
    for(i=0; i<4; ++i){
        num += (sections[i] <<(8*i));
    }
    return num;
}

//return: 1 if ip is the local IP
//        0 otherwise
int is_local_IP(unsigned int ip){
/*
    char buffer[20];
    unsigned char *bytes = (unsigned char *) &ip;    
    sprintf (buffer, "%d.%d.%d.%d",    
                    bytes[0], bytes[1], bytes[2], bytes[3]);    
    return strncmp(&LOCAL_IP, buffer, strlen(buffer));
*/
    return ip == LOCAL_IP_NUM;
};

int num_to_ip(unsigned int ip32, char* ip){
	unsigned char *bytes = (unsigned char *) &ip32;
	sprintf (ip, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}
