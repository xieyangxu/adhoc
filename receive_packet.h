#include <stdio.h>
#include "header.h"

//brief: when we receive a packet, this function would be called to handle it
//parameter:
//          packet: the whole packet you received
//          packet_len: the length of packet
void receive_packet(char *packet, int packet_len);
void print_packet(char* packet_content);
int on_receive_DATA(DATA* received_packet);
void print_DATA(DATA* received_packet_data);