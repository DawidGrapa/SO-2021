#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int signal_counter = 0,sig1=SIGUSR1,sig2=SIGUSR2,mode;

void handler(int sig,siginfo_t *info,void*context){
    if(sig == sig2){
        if(mode ==2){
            union sigval sigval;
            sigval.sival_int = 0;
            for(int i=0;i<signal_counter;i++){
                sigqueue(info->si_pid,sig1,sigval);
            }
            sigqueue(info->si_pid,sig2,sigval);
            }
        else if(mode == 1 || mode == 3){
            for(int i=0; i<signal_counter; i++){
                kill(info->si_pid,sig1);
            }
            printf("Received %d signals in catcher",signal_counter);
            kill(info->si_pid,sig2);
        }
    }
}
void count_handler(int signo){
    signal_counter++;
}

int main(int argc, char* argv[]){

    if(argc<2) {
        puts("Wrong arguments"); 
        return 0;
    }

    if(!strcmp(argv[1],"sigrt")){
        sig1=SIGRTMIN;
        sig2=SIGRTMIN+1;
        mode=3;
    }
    else if(!strcmp(argv[1],"sigqueue")){
        mode = 2;
    }
    else if(!strcmp(argv[1],"kill")) mode = 1;
    else return 1;

    printf("Chatcher PID: %d\n",getpid());
    struct sigaction act1;
    struct sigaction act2;
    act2.sa_flags = SA_SIGINFO;
    act1.sa_sigaction = count_handler;
    act2.sa_sigaction = handler;


    sigaction(sig1,&act1,NULL);
    sigaction(sig2,&act2,NULL);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,sig1);
    sigdelset(&mask,sig2);

    while(1) sigsuspend(&mask);
    return 0;
}