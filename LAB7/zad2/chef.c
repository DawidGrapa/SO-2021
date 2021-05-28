#include "shared.h"

bake* pizzeria_bake;
table *pizzeria_table;

sem_t *sem_bake;
sem_t *sem_table;

int rand_number(int from, int to){
    return rand() %(to-from+1)+from;
}

void handler(int sig){
    munmap(pizzeria_bake, sizeof(pizzeria_bake));
    munmap(pizzeria_table, sizeof(pizzeria_table));
    sem_close(sem_bake);
    sem_close(sem_table);
    exit(0);
}

int main(int argc, char *argv[]){
    srand(time(NULL)^(getpid()<<16));
    signal(SIGINT, handler);

    sem_bake = sem_open(BAKE_SEM, 0);
    sem_table=sem_open(TABLE_SEM, 0);

    pizzeria_bake=add_memory_block(BAKE_PATH, sizeof(pizzeria_bake));
    pizzeria_table=add_memory_block(TABLE_PATH, sizeof(pizzeria_table));


    while(1){
        int type = rand_number(1,9);
        sleep(1);
        printf("%d %sPrzygotowuje pizze: %d\n\n",getpid(), timestamp(), type);
        sem_wait(sem_bake);
        while(pizzeria_bake->pizzas_no==BAKE_SPACE){
            sem_post(sem_bake);
            sleep(1);
            sem_wait(sem_bake);
        }
        int index = pizzeria_bake->next_free_space;
        pizzeria_bake->bake_space[index]=type;
        pizzeria_bake->next_free_space=(pizzeria_bake->next_free_space+1) % BAKE_SPACE;

        pizzeria_bake->pizzas_no+=1;

        printf("%d %sDodalem pizze: %d. Liczba pizz w piecu: %d\n\n", getpid(), timestamp(), type, pizzeria_bake->pizzas_no);

        sem_post(sem_bake);
        sleep(5);   
        sem_wait(sem_bake);

        pizzeria_bake->bake_space[index]=-1;
        pizzeria_bake->pizzas_no-=1;

        printf("%d %sWyjalem pizze: %d. Liczba pizz w piecu: %d\n\n",getpid(), timestamp(), type, pizzeria_bake->pizzas_no);
        sem_post(sem_bake);

        sem_wait(sem_table);

        while(pizzeria_table->pizzas_no==TABLE_SPACE){
            sem_post(sem_table);
            sleep(1);
            sem_wait(sem_table);
        }
        pizzeria_table->table_space[pizzeria_table->next_free_space]=type;
        pizzeria_table->pizzas_no+=1;
        pizzeria_table->next_free_space=(pizzeria_table->next_free_space+1)%TABLE_SPACE;

        printf("%d %sUmieszczam pizze na stole: %d. Liczba pizz na stole: %d\n\n", getpid(), timestamp(), type, pizzeria_table->pizzas_no);

        sem_post(sem_table);
    }
}