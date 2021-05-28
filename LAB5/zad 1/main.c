#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define size 100
int max_components = 10;
size_t LINE_SIZE = 100;
int NO_COMPONENTS=0;


struct Component{
    char name[size];
    char *commands[size][size];
    int no_commands;
};

struct Component *parse_lines(FILE *file){
    struct Component *components = malloc(max_components*sizeof(struct Component));
    char text[size];
    for(int i=0;i<max_components;i++){
        for(int j=0;j<size;j++){
            for(int k=0;k<size;k++){
                components[i].commands[j][k]=NULL;
            }
        }
    }
    int i=0;
    while(fgets(text,size,file) !=NULL){
        if(!strcmp(text,"\n")) break;
        char *temp = strtok(text," ");
        strcpy(components[i].name,temp);
        NO_COMPONENTS++;
        temp=strtok(NULL," \n");
        int a = 0;
        int b = 0;
        while(temp!=NULL){
            if(temp[0]=='|' && temp[1]=='\0'){
                a++;
                b=0;
            }
            else if(temp[0]=='=' && temp[1]=='\0'){

            }
            else{
                components[i].commands[a][b] = malloc(sizeof(char)*(strlen(temp)+1));
                strcpy(components[i].commands[a][b],temp);
                b++;
            }
            temp=strtok(NULL," \n");
        }
        components[i].no_commands = a+1;
        i++;
    }
    return components;
}

int get_command_id(struct Component *comp, char *name){
    for(int i=0;i<NO_COMPONENTS;i++){
        if(!strcmp(comp[i].name,name)){
            return i;
        }
    }
    return -1;
}

int main(int argc, char*argv[]){
    if(argc!=2){
        puts("Wrong arguments");
        exit(0);
    }
    FILE *file = fopen(argv[1],"r");
    if (file == NULL){
        printf("Error while opening file.");
        exit(0);
    }

    struct Component *components = parse_lines(file);
    pid_t child;
    int status,fd_1[2],fd_2[2];

    char text[size];
    while(fgets(text,size,file)!=NULL){
        char *temp = strtok(text," \n");
        int sum=0,command_list[max_components],current = 0;
        while(temp!=NULL){
            if(temp[0]!='|'){
                int id = get_command_id(components,temp);
                command_list[current++] = id;
                sum+=components[id].no_commands;
            }
            temp = strtok(NULL," \n");
        }
        int started = 0;
        for(int j=0;j<current;j++){
            for(int i=0;i<components[command_list[j]].no_commands;i++){
                if (pipe(fd_2)==-1){
                    exit(EXIT_FAILURE);
                }
                child = fork();
                if(child ==0){
                    if(started>0){
                        dup2(fd_1[0], STDIN_FILENO);
                        close(fd_1[1]);
                    }
                    if(started+1<sum){
                        dup2(fd_2[1],STDOUT_FILENO);
                        close(fd_2[0]);
                    }
                }
                else{
                    if(started>0){
                        close(fd_1[0]);
                        close(fd_1[1]);
                    }
                    if(started+1<sum){
                        fd_1[0]=fd_2[0];
                        fd_1[1]=fd_2[1];
                    }
                }
                started++;
            }
        }
        while((wpid = wait(&status))>0);
    }
    for(int i=0;i<max_components;i++){
        for(int j=0;j<size;j++){
            for(int k=0;k<size;k++){
                free(components[i].commands[j][k]);
            }
        }
    }
    free(components);
}   