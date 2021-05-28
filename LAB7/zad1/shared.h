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


#define PIZZERIA_pATH "pizzeria"
#define BAKE_PATH "bake"
#define TABLE_PATH "table"

#define PIZZERIA_ID 1
#define BAKE_ID 2
#define TABLE_ID 3

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

int get_shared_block(char *file, int size, int id);
void *add_memory_block(char *file, int size, int id);
void block(int sem_id, struct sembuf* sb);
void unblock(int sem_id, struct sembuf* sb);

char *timestamp();


#endif