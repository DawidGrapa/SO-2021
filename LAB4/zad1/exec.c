#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handler(int signo){
    printf("    Got signal number %d from %d process\n",signo,getpid());
    return;
}

void ignore(){
    puts("\nIGNORE TEST\n");
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigaction(SIGUSR1,&act,NULL);

    raise(SIGUSR1);

    char *args[] = {"./exec_rec","ignore",NULL};
    execvp(args[0],args);
}


void mask(){
    puts("\nMASK TEST\n");
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);
    sigprocmask(SIG_SETMASK,&mask,NULL);

    raise(SIGUSR1);

    char *args[] = {"./exec_rec","mask",NULL};
    execvp(args[0],args);
}


void pending(){
    puts("\nPENDING TEST\n");

    sigset_t mask; //zbior sygnalow
    sigemptyset(&mask);  //inicjalizacja pustego zbioru
    sigaddset(&mask,SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);  //nadanie maski na aktualny proces

    raise(SIGUSR1);

    sigset_t waiting;
    sigpending(&waiting);
    if(sigismember(&waiting,SIGUSR1)){
        printf("    Pending in parent\n");
    }

    char *args[] = {"./exec_rec","pending",NULL};
    execvp(args[0],args);

    puts("      There is only pending in parent so child doesn't get pending signals.");
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