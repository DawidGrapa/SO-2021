#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

int ELVES_NO = 10;
int RENIFERS_NO = 9;
const int MAX_PROBLEMS = 3;




int renifers_waiting_no = 0;
int elves_waiting_no = 0;
int problems_solving = 1;
int presents_delivering = 1;
int santa_sleeps = 1;

pthread_t elves_waiting[3];

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t presents_delivering_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t presents_delivering_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t elves_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t elves_waiting_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t problems_solving_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t problems_solving_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t renifers_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;

void *santa(void *arg){
    int trips = 0;
    while(trips<3){
        pthread_mutex_lock(&santa_mutex);
        while(santa_sleeps){
            pthread_cond_wait(&santa_cond, &santa_mutex);
        }
        printf("Mikolaj budze sie\n");
        pthread_mutex_lock(&renifers_waiting_mutex);
        if(renifers_waiting_no==RENIFERS_NO){
            renifers_waiting_no=0;
            printf("Mikolaj: dostarczam zabawki\n");
            trips++;
            sleep(rand()%3+2);
            pthread_mutex_lock(&presents_delivering_mutex);
            presents_delivering=0;
            pthread_cond_broadcast(&presents_delivering_cond);
            pthread_mutex_unlock(&presents_delivering_mutex);
            pthread_mutex_unlock(&renifers_waiting_mutex);
        }
        else{
            pthread_mutex_unlock(&renifers_waiting_mutex);
            printf("Mikolaj rozwiazuje problemu elfow: %lu %lu %lu\n", elves_waiting[0], elves_waiting[1], elves_waiting[2]);
            sleep(rand()%2+1);
            pthread_mutex_lock(&problems_solving_mutex);
            pthread_mutex_lock(&elves_waiting_mutex);
            elves_waiting_no=0;
            problems_solving=0;
            pthread_cond_broadcast(&elves_waiting_cond);
            pthread_cond_broadcast(&problems_solving_cond);
            pthread_mutex_unlock(&problems_solving_mutex);
            pthread_mutex_unlock(&elves_waiting_mutex);
        }
        printf("Mikolaj: zasypiam\n");
        santa_sleeps=1;
        pthread_mutex_unlock(&santa_mutex);
    }
    printf("Koniec roboty, fajrancik\n");
    return NULL;
}

void *renifers(void *arg){
    pthread_t id = pthread_self();

    while(1){
        sleep(rand()%6+5);

        pthread_mutex_lock(&renifers_waiting_mutex);
        printf("Renifer %lu: czeka %d reniferow na Mikolaja\n", id, ++renifers_waiting_no);
        if(renifers_waiting_no==RENIFERS_NO){
            pthread_mutex_unlock(&renifers_waiting_mutex);
            printf("Refiner %lu: budze Mikolaja\n", id);
            pthread_mutex_lock(&santa_mutex);
            santa_sleeps=0;
            pthread_cond_broadcast(&santa_cond);
            pthread_mutex_unlock(&santa_mutex);
        }
        else{
            pthread_mutex_unlock(&renifers_waiting_mutex);
        }

        presents_delivering=1;
        pthread_mutex_lock(&presents_delivering_mutex);
        while(presents_delivering){
            pthread_cond_wait(&presents_delivering_cond, &presents_delivering_mutex);
        }
        pthread_mutex_unlock(&presents_delivering_mutex);
    }
    return NULL;
}


void *elves(void *arg){
    pthread_t id = pthread_self();
    while(1){
        sleep(rand()%4 + 2);
        pthread_mutex_lock(&elves_waiting_mutex);
        while(elves_waiting_no == MAX_PROBLEMS){
            printf("Elf %lu: czekam na powrot elfow\n", id);
            pthread_cond_wait(&elves_waiting_cond, &elves_waiting_mutex);
        }
        elves_waiting[elves_waiting_no++]=id;
        printf("Elf %lu: czeka %d elfow na Mikolaja\n", id, elves_waiting_no);

        if(elves_waiting_no==3){
            printf("Elf %lu wybudzam Mikolaja\n", id);
            pthread_mutex_unlock(&elves_waiting_mutex);
            pthread_mutex_lock(&santa_mutex);
            santa_sleeps=0;
            pthread_cond_broadcast(&santa_cond);
            pthread_mutex_unlock(&santa_mutex);
        }
        else{
            pthread_mutex_unlock(&elves_waiting_mutex);
        }

        pthread_mutex_lock(&problems_solving_mutex);
        problems_solving = 1;
        while(problems_solving){
            pthread_cond_wait(&problems_solving_cond, &problems_solving_mutex);
        }
        pthread_mutex_unlock(&problems_solving_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    pthread_t threads[ELVES_NO+RENIFERS_NO+1];

    pthread_create(&threads[0], NULL, santa, NULL);

    for(int i=1;i<RENIFERS_NO+1;i++){
        pthread_create(&threads[i], NULL, renifers, NULL);
    }
    for(int i=RENIFERS_NO+1;i<ELVES_NO+RENIFERS_NO+1;i++){
        pthread_create(&threads[i],NULL,elves,NULL);
    }
    for(int i=0;i<ELVES_NO+RENIFERS_NO+1;i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}