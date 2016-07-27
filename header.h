#include <stdio.h>

/*
* There are structures of the header in different packets. 
* This is a really tricky way to use the struct below.
* You can use it directly or change it as you like.
*/

#ifndef __header__
#define __header__

//type of packets
#define TYPE_DATA '1'
#define TYPE_RREQ '2'
#define TYPE_RREP '3'
#define TYPE_RERR '4'

//There are only three computers are required in this project, 
//so we assume that setting the address list length to 8 is enough. 
//You can change it if you work in a bigger ad-hoc network
#define MAX_ADDR_NUM 8
#define MAX_DATA_LEN 1024

//Route discovery
typedef struct{
    int request_id;
    unsigned int dest_addr;
    int addr_num;	//The number of address which make sense in the following array
    unsigned int addr[MAX_ADDR_NUM];
}RREQ;

//Route reply
typedef struct{
    unsigned int dest_addr;
    int addr_num;
    unsigned int addr[MAX_ADDR_NUM];
}RREP;

//Data packet
typedef struct{
    int addr_num;
    int data_len;
    unsigned int addr[MAX_ADDR_NUM];
    char data[MAX_DATA_LEN];
}DATA;

#endif
