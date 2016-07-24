#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RID_TABLE 64
#define MAX_LOCAL_RID_TABLE 64

//node to record the request id for each (src, dst) pair
typedef struct rid_node{
    unsigned int dst;
    unsigned int src;
    unsigned int req_id;
}RID_NODE;

//node to record the request id for each dst
typedef struct local_rid_node{
    unsigned int dst;
    unsigned int req_id;
}LOCAL_RID_NODE;

int rid_table_iter = 0;
int local_rid_table_iter = 0;

RID_NODE req_id_table[MAX_RID_TABLE];
LOCAL_RID_NODE local_req_id_table[MAX_LOCAL_RID_TABLE];

void init_flood(){
    memset(req_id_table, 0, sizeof(req_id_table));
    memset(local_req_id_table, 0, sizeof(local_req_id_table));
    rid_table_iter = 0;
    local_rid_table_iter = 0;
};

unsigned int get_unique_id(unsigned int dst){
    int i=0;
    for(i=0; i<MAX_LOCAL_RID_TABLE; ++i){
        if(local_req_id_table[i].dst == dst){     
            return ++local_req_id_table[i].req_id;
        }
    }
    if(i == MAX_LOCAL_RID_TABLE){
        local_req_id_table[local_rid_table_iter].dst = dst;
        local_req_id_table[local_rid_table_iter].req_id = 0;
        local_rid_table_iter = (local_rid_table_iter + 1) % MAX_LOCAL_RID_TABLE;
        return 0;
    }
}
int check_new(unsigned int src,unsigned int dst,unsigned int req_id){
    int i=0;
    for(i=0; i<MAX_RID_TABLE; ++i){
        if(req_id_table[i].dst == dst &&
             req_id_table[i].src == src &&
             req_id_table[i].req_id == req_id){
            return -1;
        }
    }
    req_id_table[rid_table_iter].dst = dst;
    req_id_table[rid_table_iter].src = src;
    req_id_table[rid_table_iter].req_id = req_id;
    rid_table_iter = (rid_table_iter + 1) % MAX_RID_TABLE;
    return 0;
}
