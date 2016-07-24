#include <stdio.h>
#include <stdlib.h>     
#include <time.h>     
#include <signal.h>    
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/vfs.h>    
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>        
#include <string.h>        
#include <unistd.h>     

#include "socket.h"
#include "receive_packet.h"
#include "header.h"
#include "flood.h"
#include "cache.h"

//the socket for sending
int sock;
struct sockaddr_in sock_in;

#define SOCK_IN 9999
#define SOCK_OUT 9998

int get_local_ip(){    
    struct ifaddrs * ifAddrStruct=NULL;    
    void * tmpAddrPtr=NULL;    
    int flag = 0;
    getifaddrs(&ifAddrStruct);    
    
    while (ifAddrStruct!=NULL){    
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET){    
            // check if it is a valid IP4 Address    
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;    
            char addressBuffer[INET_ADDRSTRLEN];    
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN); 
            //change the following code by modifying the "wlan0"
            if(strcmp(ifAddrStruct->ifa_name,"wlan0")==0 || strcmp(ifAddrStruct->ifa_name,"wlp4s0")==0) {
                printf("Local IP Address %s\n", addressBuffer); 
                local_ip_num = ip_to_num(addressBuffer);
                strcpy(local_ip,addressBuffer); 
                flag = 1;
            }    
        }    
        ifAddrStruct = ifAddrStruct->ifa_next;    
    }    
    if(flag == 0){
        printf("***error! Cannot get local IP address! \n");
	printf("***Please open ad-hoc mode or modify the get_local_ip() in socket.c\n");
        return -1;
    }
    return 0;
}

int send_handler(char *IP, char *send_str){
    if(strncmp(IP,"162.105.1.",10)!=0){
        printf("IP address error!");
        return -1;
    }
    unsigned int IP32 = ip_to_num(IP);
    int status = send_packet(IP32, send_str, strlen(send_str));
    printf("send succeed\n",status);
    return status;
}

int receive_handler(char *packet,int packet_len){
    char buffer[MAXBUF];
    char dst_IP[IP_LENGTH];
    memset(buffer, 0, sizeof(buffer));
    memset(dst_IP, 0, sizeof(dst_IP));   
    strncpy(dst_IP, packet, IP_LENGTH);
    strncpy(buffer, packet+IP_LENGTH, packet_len-IP_LENGTH);
    
    if((strcmp(&LOCAL_IP,dst_IP) != 0) && (strcmp(BROADCAST_IP,dst_IP) != 0)){
         return -1;
    }
    printf("receive a packet!!!\n");
    receive_packet(buffer,packet_len+1-IP_LENGTH);
}

void *socket_receive(){
    int sock, status, buflen,peerlen;
    unsigned sinlen;
    char buffer[MAXBUF];
    struct sockaddr_in sock_in,peer_Addr;
    int yes = 1;

    sinlen = sizeof(struct sockaddr_in);
    memset(&sock_in, 0, sinlen);

    sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if((sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        printf("sock error!!! sock = %d\n", sock);
        return ;
    }

    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_in.sin_port = htons(SOCK_IN);
    sock_in.sin_family = PF_INET;

    if((status = bind(sock, (struct sockaddr *)&sock_in, sinlen)) < 0){
        printf("***Error! Bind error!!! Status = %d\n", status);
        return;
    }

    //status = getsockname(sock, (struct sockaddr *)&sock_in, &sinlen);
    printf("Sock port %d\n",htons(sock_in.sin_port));
    //status = getpeername(sock,(struct sockaddr *)&peer_Addr,&peerlen);

    buflen = MAXBUF;
    while(1){
        memset(buffer, 0, buflen);
        if((status = recvfrom(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, &sinlen)) == -1){
            printf("***Error occured when receive!");
        }
        receive_handler(buffer,status);
    }
    shutdown(sock, 2);
    close(sock);
    return;
}

int send_socket_packet(char * packet,int packet_len){
    int status = sendto(sock, packet, packet_len, 0, (struct sockaddr *)&sock_in, sizeof(struct sockaddr_in));
    return status;
}

void open_send_socket(){
    int status, buflen, sinlen;
    char buffer[MAXBUF];
    int yes = 1;

    sinlen = sizeof(struct sockaddr_in);
    memset(&sock_in, 0, sinlen);
    buflen = MAXBUF;

    sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_in.sin_port = htons(SOCK_OUT);
    sock_in.sin_family = PF_INET;

    if((status = bind(sock, (struct sockaddr *)&sock_in, sinlen))<0)
        printf("***Error: Bind Status = %d\n", status);

    if((status = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) ) )<0)
        printf("***Error: Setsockopt Status = %d\n", status);

    sock_in.sin_addr.s_addr=htonl(255 + 256 + 256*256*105 + 256*256*256*162); /* send message to 162.105.1.255 */
    sock_in.sin_port = htons(SOCK_IN); /* port number */
    sock_in.sin_family = PF_INET;
}
int main(){
    int status = get_local_ip();
    if(status < 0) return 0;

    init_flood();
    init_path_table();	
    open_send_socket();
	
    pthread_t thread_receive;
    pthread_create(&thread_receive,NULL,socket_receive,NULL);
    
    char ip[20];
    char buffer[MAXBUF];

    while(1){
        printf("please input target ip address:\n");
        if(!gets(ip))break;
        printf("please input the data:\n");
        if(!gets(buffer))break;
        send_handler(ip,buffer);
    }

    shutdown(sock, 2);
    close(sock);
    return 0;
}
