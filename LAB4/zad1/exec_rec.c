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
    raise(SIGUSR1);
}

void mask(){
    raise(SIGUSR1);
}


void pending(){
    sigset_t waiting;
    sigpending(&waiting);
    if(sigismember(&waiting,SIGUSR1)){
        printf("Pending in child");
    }
}

int main(int argc, char*argv[]){
    if(argc<2){
        puts("Not enough arguments");
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