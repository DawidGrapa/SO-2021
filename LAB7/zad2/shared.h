#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>


#define BAKE_PATH "/bake"
#define TABLE_PATH "/table"

#define BAKE_SEM "/bake_sem"
#define TABLE_SEM "/table_sem"



#define BAKE_SPACE 5
#define TABLE_SPACE 5

typedef struct bake{
    int bake_space[BAKE_SPACE];
    int pizzas_no;
    int next_free_space;
} bake;

typedef struct table{
    int table_space[TABLE_SPACE];
    int pizzas_no;
    int next_free_space;
    int pizza_to_be_delivered;
} table;

union semun{
    int val;
    struct semid_ds *buf;
    short *array;
};

int get_shared_block(char *file, int size);
void *add_memory_block(char *file, int size);

char *timestamp();


#endif