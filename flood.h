#include <stdio.h>

//brief: get increased id for specific destination
unsigned int get_unique_id(unsigned int dst);

//brief: check if a packet has been received using its information
int check_new(unsigned int src,unsigned int dst, unsigned int req_id);
void init_flood();
