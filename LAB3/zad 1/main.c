#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char * argv[]){
    if(argc<2){
        puts("Not enough arguments");
        return 0;
    }
    int n = atoi(argv[1]);
    
    
    for(int i=0;i<n;i++){
        if(fork() == 0){
            printf("Process nr %d\n",getpid());
            exit(0);
        }
    }
    
    for(int i=0;i<n;i++){
        wait(NULL);
    }

    return 0;
}