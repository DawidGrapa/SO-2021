#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handler(int signo,siginfo_t *info, void *context){  //sa_siginfo musi miec osobny handler bo bierze 3 argumenty a nie 1
    printf("    Got the signal %d, process PID %d\n",signo,getpid());
    printf("    Data in siginfo:\n");
    printf("    Signal number: %d\n",info->si_signo);
    printf("    Signal code: %d\n",info->si_code);
    printf("    Process ID: %d\n",info->si_pid);
    printf("    System time taken: %d\n",(int)info->si_stime);
    printf("    User time taken: %d\n",(int)info->si_utime);
}

void flag_handler(int signo){
    printf("    Handled signal number: %d from proces number: %d\n",signo,getpid());
}

void flag_1(){
    puts("\nFIRST FLAG: SA_SIGINFO");
    struct sigaction act;
    act.sa_sigaction = handler; //sa_sigaction poniewaz sa_siginfo
    act.sa_flags=SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);
    raise(SIGUSR1);
}

void flag_2(){
    puts("\nSECOND FLAG: SA_NOCLDSTOP");
    struct sigaction act;
    act.sa_handler = flag_handler;
    pid_t child = fork();
    act.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act, NULL);
    if(child == 0) {
        while(1);
    } else {
        kill(child, SIGSTOP);
    }
    child = fork();
    if(child  == 0) {
        while(1);
    } else {
        kill(child, SIGKILL);
        wait(NULL);
    }

    
}

void flag_3(){
    puts("\nTHIRD FLAG: SA_NOCLDWAIT");
    struct sigaction act;
    act.sa_handler = flag_handler;
    act.sa_flags=SA_NOCLDWAIT;
    sigaction(SIGCHLD,&act,NULL);

    pid_t child = fork();
    if(fork == 0 ) while(1);
    else{
        kill(child,SIGKILL);
        if(wait(NULL)>0) puts("There was a zombie");
    }
}



int main(){
    flag_1();
    flag_2();
    flag_3();
    return 0;
}