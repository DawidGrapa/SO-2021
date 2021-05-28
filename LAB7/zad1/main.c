#include "shared.h"

int N, M;
pid_t *staff;

void handler(){
    for(int i=0; i<(N+M); i++){
        kill(staff[i], SIGINT);
    }
    free(staff);
}

void open_pizzeria(){
    //klucz do semafora
    key_t sem_key = ftok(PIZZERIA_pATH, PIZZERIA_ID);
    //zwraca id nowoutworzonego semafora
    int semaphore_id = semget(sem_key, 2, 0666 | IPC_CREAT);
    //unia potrzbna do inicjalizacji (ustawioamy wartosc podana w arg.val)
    union semun arg;
    arg.val = 1;
    semctl(semaphore_id, 0, SETVAL, arg);
    semctl(semaphore_id, 1, SETVAL, arg);
    //tworze pamiec wspoldzielona dla pieca
    bake *pizzeria_bake = add_memory_block(BAKE_PATH, sizeof(bake), BAKE_ID);

    pizzeria_bake->pizzas_no=0;
    pizzeria_bake->next_free_space=0;
    for(int i=0;i<BAKE_SPACE;i++){
        pizzeria_bake->bake_space[i]=-1;
    }
    //to samo dla stolu
    table *pizzeria_table = add_memory_block(TABLE_PATH, sizeof(table), TABLE_ID);
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


    int bake_to_unlock = get_shared_block(BAKE_PATH, 0, BAKE_ID);
    int table_to_unlock = get_shared_block(TABLE_PATH, 0, TABLE_ID);
    shmctl(bake_to_unlock, IPC_RMID, NULL);
    shmctl(table_to_unlock, IPC_RMID, NULL);
    semctl(semaphore_id, 0, IPC_RMID, NULL);
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