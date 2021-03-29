#define  _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <dirent.h>

char MAIN_DIR[512];
int MAX_DEPTH=0;
static int FORK_COUNT = 0;
const int BUFFOR = 512;

int check_if_txt(char *file){
    size_t len=strlen(file);
    if(file[len-1]=='t' && file[len-2]=='x' && file[len-3]=='t' && file[len-4] == '.'){
        return 1;
    }
    return 0;
}

int contain(char *file, char* word){
    FILE *f = fopen(file,"r");
    if(f == NULL) return 0;
    char text[BUFFOR];
    
    while(fgets(text,BUFFOR,f)){
        if(strstr(text,word)!=NULL) return 1;
    }
    
    return 0;
}

void recursion(char *directory, char* word,int depth){
    DIR *dir = opendir(directory);
    if(dir == NULL){
        exit(0);
    }

    struct dirent* dirent_dir = readdir(dir);
    
    while(dirent_dir != NULL){
        if(dirent_dir->d_type == DT_REG && strlen(dirent_dir->d_name)>=5 && check_if_txt(dirent_dir->d_name)){
            char path[512]="";
            snprintf(path,sizeof(path),"%s/%s", directory, dirent_dir->d_name);
            if(contain(path,word)){
                snprintf(path,sizeof(path),"%s/%s", directory, dirent_dir->d_name);
                printf("Process pid: %d\nDirectory to file: %s\n",getpid(),path);
            }
            
        }
        else if(depth<MAX_DEPTH && DT_DIR == dirent_dir->d_type && strcmp("..",dirent_dir->d_name) && strcmp(".",dirent_dir->d_name)){
            if(fork()==0){
                FORK_COUNT++;
                char path[512]="";
                snprintf(path,sizeof(path),"%s/%s", directory, dirent_dir->d_name);
                closedir(dir);
                recursion(path,word,depth+1);
                exit(0);
            }
        }
        dirent_dir = readdir(dir);
    }

    closedir(dir);
}

int main(int argc, char **argv){

    if(argc<4){
        puts("Not enough arguments.");
        return 0;
    }

    strcat(MAIN_DIR,argv[1]);
    MAX_DEPTH = atoi(argv[3]);

    recursion(MAIN_DIR,argv[2],0);

    for(int i=0;i<=FORK_COUNT;i++){
        wait(NULL);
    }

    return 0;
}