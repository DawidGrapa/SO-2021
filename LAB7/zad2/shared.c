#include "shared.h"

int get_shared_block(char *file, int size){
    int res=shm_open(file, O_RDWR | O_CREAT, 0760);
    ftruncate(res, size);
    return res;
}

void *add_memory_block(char *file, int size){
    int shared_id = get_shared_block(file, size);
    
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_id, 0);
}

char *timestamp(){
    time_t date = time(NULL);
    char *res_time = asctime(gmtime(&date));
    return res_time;
}