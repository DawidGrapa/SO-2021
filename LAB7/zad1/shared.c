#include "shared.h"

int get_shared_block(char *file, int size, int id){
    key_t key;
    key = ftok(file, id);
    return shmget(key, size, 0644 | IPC_CREAT);
}

void *add_memory_block(char *file, int size, int id){
    int shared_id = get_shared_block(file, size, id);
    char *result = shmat(shared_id, NULL, 0);
    return result;
}

void block(int sem_id, struct sembuf* sb){
    sb->sem_op=-1;
    semop(sem_id, sb, 1);
}

void unblock(int sem_id, struct sembuf* sb){
    sb->sem_op=1;
    semop(sem_id, sb, 1);
}

char *timestamp(){
    time_t date = time(NULL);
    char *res_time = asctime(gmtime(&date));
    return res_time;
}