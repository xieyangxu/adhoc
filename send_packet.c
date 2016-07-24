#include <stdio.h>
#include "send_packet.h"
#include "utils.h"
#include "header.h"
#include "flood.h"

int send_packet(unsigned int ip, char *data,int data_len){
    int status = send_unicast(ip, data, data_len);
    //Todo

    return status;
}

