#include "shared.h"

bake* pizzeria_bake;
table *pizzeria_table;

int rand_number(int from, int to){
    return rand() %(to-from+1)+from;
}

void handler(int sig){
    shmdt(pizzeria_bake);
    shmdt(pizzeria_table);
    exit(0);
}

int main(int argc, char *argv[]){
    signal(SIGINT, handler);
    key_t sem_key = ftok(PIZZERIA_pATH, PIZZERIA_ID);

    int sem_id = semget(sem_key, 0, 0);

    pizzeria_bake=add_memory_block(BAKE_PATH, 0, BAKE_ID);
    pizzeria_table=add_memory_block(TABLE_PATH, 0, TABLE_ID);

    struct sembuf sem_bake;
    sem_bake.sem_flg=0;
    sem_bake.sem_num=0;
    sem_bake.sem_op=-1;

    struct sembuf sem_table;
    sem_table.sem_op=-1;
    sem_table.sem_flg=0;
    sem_table.sem_num=1;

    while(1){
        srand(time(NULL)^(getpid()<<16));
        int type = rand_number(1,9);
        sleep(1);
        printf("%d %sPrzygotowuje pizze: %d\n\n",getpid(), timestamp(), type);
        block(sem_id, &sem_bake);

        while(pizzeria_bake->pizzas_no==BAKE_SPACE){
            unblock(sem_id, &sem_bake);
            sleep(1);
            block(sem_id, &sem_bake);
        }
        int index = pizzeria_bake->next_free_space;
        pizzeria_bake->bake_space[index]=type;
        pizzeria_bake->next_free_space=(pizzeria_bake->next_free_space+1) % BAKE_SPACE;

        pizzeria_bake->pizzas_no+=1;

        printf("%d %sDodalem pizze: %d. Liczba pizz w piecu: %d\n\n", getpid(), timestamp(), type, pizzeria_bake->pizzas_no);

        unblock(sem_id, &sem_bake);
        sleep(5);
        block(sem_id, &sem_bake);

        pizzeria_bake->bake_space[index]=-1;
        pizzeria_bake->pizzas_no-=1;

        printf("%d %sWyjalem pizze: %d. Liczba pizz w piecu: %d\n\n",getpid(), timestamp(), type, pizzeria_bake->pizzas_no);
        unblock(sem_id, &sem_bake);

        block(sem_id, &sem_table);

        while(pizzeria_table->pizzas_no==TABLE_SPACE){
            unblock(sem_id, &sem_table);
            sleep(1);
            block(sem_id, &sem_table);
        }

        pizzeria_table->table_space[pizzeria_table->next_free_space]=type;
        pizzeria_table->pizzas_no+=1;
        pizzeria_table->next_free_space=(pizzeria_table->next_free_space+1)%TABLE_SPACE;

        printf("%d %sUmieszczam pizze na stole: %d. Liczba pizz na stole: %d\n\n", getpid(), timestamp(), type, pizzeria_table->pizzas_no);

        unblock(sem_id, &sem_table);
    }
}