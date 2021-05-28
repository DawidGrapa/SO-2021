#include <stdio.h>
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int signal_counter;

int no_signals = 0;

int mode,last_received,sig1,sig2;

void handler(int sig,siginfo_t *info,void*context){
    if(mode == 2) last_received = info->si_value.sival_int;
    if(sig == sig1){
        signal_counter++;
    }
    else if(sig == sig2){
        printf("    Received %d signals, expected %d signals\n",signal_counter,no_signals);
        if(mode==2)printf("    Last received signal: %d\n",last_received);
        exit(0);
    }
}



int main(int argc, char*argv[]){
    printf("Sender: %d",getpid());
    signal_counter = 0;
    if(argc<3) {puts("Wrong arguments");return 0;};

    if(!strcmp(argv[3],"sigrt")){
        sig1=SIGRTMIN;
        sig2=SIGRTMIN+1;
        mode=3;
    }
    else if(!strcmp(argv[3],"sigqueue")){
        mode = 2;
        sig1=SIGUSR1;
        sig2=SIGUSR2;
    }
    else if(!strcmp(argv[3],"kill")) {
        mode = 1;
        sig1=SIGUSR1;
        sig2=SIGUSR2;
    }
    else return 1;
    pid_t catcher_pid = atoi(argv[1]);
    
    no_signals = atoi(argv[2]);

    struct sigaction act;
    act.sa_flags = SA_SIGINFO;

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask,sig1);
    sigdelset(&mask,sig2);

    act.sa_sigaction = handler;

    sigaction(sig1,&act,NULL);
    sigaction(sig2,&act,NULL);
    
    if(mode ==2){
        union sigval sigval;
        sigval.sival_int = 0;
        for(int i=0;i<no_signals;i++){
            sigqueue(catcher_pid,sig1,sigval);
        }
        sigqueue(catcher_pid,sig2,sigval);
    }
    else {
        for(int i=0;i<no_signals;i++){
            kill(catcher_pid,sig1);
        }
        kill(catcher_pid,sig2);
    }

    while(1)sigsuspend(&mask);
    return 0;
}