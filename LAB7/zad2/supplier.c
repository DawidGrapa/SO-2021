#include "shared.h"

table *pizzeria_table;

sem_t *sem_table;

void handler(int sig){
    munmap(pizzeria_table, sizeof(pizzeria_table));
    sem_close(sem_table);
    exit(0);
}

int main(int argc, char *argv[]){
    signal(SIGINT, handler);

    sem_table=sem_open(TABLE_SEM, 0);

    pizzeria_table = add_memory_block(TABLE_PATH, sizeof(pizzeria_table));

    while(1){
        sem_wait(sem_table);
        if(pizzeria_table->pizzas_no>0){
            int type = pizzeria_table->table_space[pizzeria_table->pizza_to_be_delivered];
            pizzeria_table->pizzas_no-=1;
            pizzeria_table->pizza_to_be_delivered=(pizzeria_table->pizza_to_be_delivered+1)%TABLE_SPACE;
            printf("%d %sPobieram pizze: %d. Liczba pizz na stole: %d\n\n", getpid(), timestamp(), type, pizzeria_table->pizzas_no);
            sem_post(sem_table);
            sleep(4);
            printf("%d %sDostarczam pizze: %d\n\n", getpid(), timestamp(), type);
            sleep(4);
        }
        else{
            sem_post(sem_table);
        }
        
    }
}