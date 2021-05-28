#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void print_sorted(char *type){
    char *command;
    printf("\nSorted by: %s\n\n",type);
    if(!strcmp(type,"sender")){
        command = "echo | mail | sort -k 3"; 
    }
    else if(!strcmp(type,"date")){
        command = "echo | mail";
    }

    FILE * file = popen(command,"r");

    if(file == NULL){
        puts("Error while using popen.");
        exit(0);
    }

    char *text;
    size_t len = 512;
    while(getline(&text,&len,file)!=EOF){
        printf("%s",text);
    }
    
}

void send_email(char * email, char* title, char* content){
    char command[512];
    sprintf(command,"echo %s | mail %s -s %s",content,email,title);

    FILE *file =popen(command, "r");

    if(file == NULL){
        puts("Error while using popen.");
        exit(0);
    }
}

int main(int argc, char *argv[]){
    if(argc!=2 && argc !=4){
        puts("Wrong arguments.");
        exit(0);
    }

    if(argc == 2){
        print_sorted(argv[1]);
    }
    else if(argc == 4){
        send_email(argv[1],argv[2],argv[3]);
    }   
}