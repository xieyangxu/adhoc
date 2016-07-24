#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cache__
#define __cache__

#define MAX_IP_CACHE 65536
#define MAX_IP_NUM 8

//node in the cache table
typedef struct{
    unsigned int dst;
    unsigned int path_len;
    unsigned int addr[MAX_IP_NUM];
}PATH_NODE;

PATH_NODE path_cache_table[];
int path_cache_iter;

//brief: init the table
//called in socket.c
void init_path_table();

//brief: insert a path information into the cache
int insert_path(unsigned int dst, unsigned int path_len, unsigned int *addr);

//brief: delete a path information from the cache table by destination address
void delete_path(unsigned int dst);

//brief: find if there are a path for specified destination address
//parameter addr: the addr has to be an unsigned int array like 'unsigned int addr[MAX_IP_NUM]'
int find_path(unsigned int dst, unsigned int *addr);

#endif
