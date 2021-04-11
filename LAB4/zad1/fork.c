#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handler(int signo){
    printf("    Got signal number %d from %d process\n",signo,getpid());
}

void ignore(){
    puts("\nIGNORE TEST\n");
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigaction(SIGUSR1,&act,NULL);

    pid_t child = fork();
    if(child!=0){
        raise(SIGUSR1);
        puts("      If there is nothing above - test completed for parent");
        wait(NULL);
    }
    else{
        raise(SIGUSR1);
        puts("      If there is nothing above - test completed for child\n");
    }
}

void handler_t(){
    puts("\nHANDLER TEST\n");
    struct sigaction act;
    act.sa_handler = handler;
    sigaction(SIGUSR1,&act,NULL);

    pid_t child = fork();
    if(child!=0){
        raise(SIGUSR1);
        wait(NULL);
    }
    else{
        raise(SIGUSR1);
        puts("      If messages are the same (except process number)- test completed succesfully.");
    }
}

void mask(){
    puts("\nMASK TEST\n");
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);
    sigprocmask(SIG_SETMASK,&mask,NULL);

    pid_t child = fork();
    if(child!=0){
        printf("If there is nothing it is ok.");
        raise(SIGUSR1);
        wait(NULL);
    }
    else{
        printf("If there is nothing it is ok.");        
        raise(SIGUSR1);
    }
}


void pending(){
    puts("\nPENDING TEST\n");

    sigset_t mask; //zbior sygnalow
    sigemptyset(&mask);  //inicjalizacja pustego zbioru
    sigaddset(&mask,SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);  //nadanie maski na aktualny proces

    raise(SIGUSR1);

    sigset_t waiting;

    pid_t child  = fork();
    if(child!=0){
        sigpending(&waiting);
        if(sigismember(&waiting,SIGUSR1)){
            puts("      Pending in parent");
            wait(NULL);
        }
    }
    else{
        sigpending(&waiting);
        if(sigismember(&waiting,SIGUSR1)){
            puts("      Pending in child");
        }
        exit(0);
    }
    puts("      There is only pending in parent so child doesn't get pending signals.");
}

int main(int argc, char*argv[]){
    if(argc<2){
        puts("Not enough arguments");
    }
    else if(!strcmp(argv[1],"handler")){
        handler_t();
    }
    else if(!strcmp(argv[1],"ignore")){
        ignore();
    }
    else if(!strcmp(argv[1],"mask")){
        mask();
    }
    else if(!strcmp(argv[1],"pending")){
        pending();
    }

    return 0;
}