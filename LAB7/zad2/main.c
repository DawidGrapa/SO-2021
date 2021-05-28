#include "shared.h"

int N, M;
pid_t *staff;

sem_t *sem_bake;
sem_t *sem_table;

void handler(){
    for(int i=0; i<(N+M); i++){
        kill(staff[i], SIGINT);
    }
    free(staff);
}

void open_pizzeria(){
    sem_bake=sem_open(BAKE_SEM, O_CREAT, 0760, 1);

    sem_table=sem_open(TABLE_SEM, O_CREAT, 0760, 1);

    bake *pizzeria_bake = add_memory_block(BAKE_PATH, sizeof(bake));

    pizzeria_bake->pizzas_no=0;
    pizzeria_bake->next_free_space=0;
    for(int i=0;i<BAKE_SPACE;i++){
        pizzeria_bake->bake_space[i]=-1;
    }
    //to samo dla stolu
    table *pizzeria_table = add_memory_block(TABLE_PATH, sizeof(table));
    pizzeria_table->pizzas_no=0;
    pizzeria_table->pizza_to_be_delivered=0;
    pizzeria_table->next_free_space=0;
    for(int i=0;i<BAKE_SPACE;i++){
        pizzeria_table->table_space[i]=-1;
    }   

    for(int i=0;i<N;i++){
        pid_t child = fork();
        staff[i]=child;
        if(child == 0){
            execlp("./chef", "./chef", NULL);
        }
    }
    for(int i=N;i<N+M;i++){
        pid_t child = fork();
        staff[i]=child;
        if(child == 0){
            execlp("./supplier", "./supplier", NULL);
        }
    }
    for(int i=0;i<N+M;i++){
        wait(NULL);
    }


    munmap(pizzeria_bake, sizeof(bake));
    shm_unlink(BAKE_PATH);

    munmap(pizzeria_table, sizeof(table));
    shm_unlink(TABLE_PATH);

    sem_close(sem_bake);
    sem_unlink(BAKE_SEM);

    sem_close(sem_table);
    sem_unlink(TABLE_SEM);
}

int main(int argc, char *argv[]){

    if(argc!=3){
        puts("Wrong arguments number!");
        exit(0);
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);

    staff = (pid_t*)malloc((N+M)*sizeof(pid_t));

    signal(SIGINT, handler);

    open_pizzeria();

    return 0;
}