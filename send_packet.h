#include <stdio.h>

//brief: when we want to send data to a specific ip address
//          this function would be called and send the data
//parameter:ip: the ip address in binary format
//          data: the message you typed in screen
//          data_len: the length of data
int send_packet(unsigned int ip, char *data,int data_len);

