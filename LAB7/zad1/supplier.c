#include "shared.h"

table *pizzeria_table;

void handler(int sig){
    shmdt(pizzeria_table);
    exit(0);
}

int main(int argc, char *argv[]){
    signal(SIGINT, handler);

    key_t sem_key = ftok(PIZZERIA_pATH, PIZZERIA_ID);
    int sem_id = semget(sem_key, 0, 0);  //flaga 0

    pizzeria_table = add_memory_block(TABLE_PATH, 0, TABLE_ID);

    struct sembuf sembuf_table;
    sembuf_table.sem_flg=0;
    sembuf_table.sem_num=1;
    while(1){
        block(sem_id, &sembuf_table);
        if(pizzeria_table->pizzas_no>0){
            int type = pizzeria_table->table_space[pizzeria_table->pizza_to_be_delivered];
            pizzeria_table->pizzas_no-=1;
            pizzeria_table->pizza_to_be_delivered=(pizzeria_table->pizza_to_be_delivered+1)%TABLE_SPACE;
            printf("%d %sPobieram pizze: %d. Liczba pizz na stole: %d\n\n", getpid(), timestamp(), type, pizzeria_table->pizzas_no);
            unblock(sem_id, &sembuf_table);
            sleep(4);
            printf("%d %sDostarczam pizze: %d\n\n", getpid(), timestamp(), type);
            sleep(4);
        }
        else{
            unblock(sem_id, &sembuf_table);
        }
        
    }
}