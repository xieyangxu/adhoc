#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"
#include "header.h"

PATH_NODE path_cache_table[MAX_IP_CACHE];
int path_cache_iter;

void init_path_table(){
    memset(path_cache_table, 0, sizeof(path_cache_table));
    path_cache_iter = 0;
}

int insert_path(unsigned int dst, unsigned int path_len, unsigned int *addr){
    int i;
    path_cache_table[path_cache_iter].dst = dst;
    path_cache_table[path_cache_iter].path_len = path_len;
    for(i=0; i<path_len; ++i){
        (path_cache_table[path_cache_iter].addr)[i] = addr[i];
    }
    ++path_cache_iter;
    path_cache_iter = path_cache_iter % MAX_IP_NUM;
}

void delete_path(unsigned int dst){
    int i;
    for(i=0; i <MAX_IP_CACHE; ++i){
        if(path_cache_table[i].dst == dst)
        path_cache_table[i].dst = 0;
        path_cache_table[i].path_len = 0;
    }
}

int find_path(unsigned int dst, unsigned int *addr){
    int i, j;
    for(i=0;i<MAX_IP_CACHE;++i){
        if(path_cache_table[i].dst == dst){
            for(j=0; j<path_cache_table[i].path_len; ++j){
                addr[j] = path_cache_table[i].addr[j];
            }
            return path_cache_table[i].path_len;
        }
    }
    return -1;
}
