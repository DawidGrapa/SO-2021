#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <time.h>

const size_t BUFFOR = 257;

double calculate_time_clocks(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

double calculate_time_tics(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void system_func(char* file, char c){
    int f=open(file,O_RDONLY);

    if(f==-1){
        puts("Error while reading file.");
        return;
    }

    char *text=malloc(BUFFOR*sizeof(char));

    size_t len=read(f,text,BUFFOR);

    size_t temp=0;
    printf("\nSYSTEM\n\n");
    while(len){
        bool wasChar=false;
        while(text[temp]!='\n'){
            if(text[temp]==c){
                wasChar=true;
            }
            temp++;
        }
        if(wasChar){
            write(1,text,temp+1);
        }
        lseek(f,temp-len+1,SEEK_CUR); //temp-value of first sign of new line - len - value of last get char value -> cofa mnie do miejsca w ktorym jest temp
        len=read(f,text,BUFFOR);
        temp=0;
    }    
    free(text);
    close(f);
}


void library(char* file,char c){
    FILE* f=fopen(file,"r+");

    if(f==NULL){
        puts("Error while reading file.");
        return;
    }
    printf("LIBRARY\n\n");

    char *text=malloc(BUFFOR*sizeof(char));

    size_t len=fread(text,sizeof(char),BUFFOR,f);

    size_t temp=0;

    while(len){
        bool wasChar=false;
        while(text[temp]!='\n'){
            if(text[temp]==c){
                wasChar=true;
            }
            temp++;
        }
        if(wasChar){
            fwrite(text,sizeof(char),temp+1,stdout);
        }
        fseek(f,temp-len+1,1); //temp-value of first sign of new line - len - value of last get char value -> cofa mnie do miejsca w ktorym jest temp
        len=fread(text,sizeof(char),BUFFOR,f);
        temp=0;
    }    
    free(text);
    fclose(f);
}

int main(int argc,char **argv){

    if(argc<3){
        puts("Not enough arguments.");
        return 0;
    }

    char c,*file1=malloc(100);

    c= argv[1][0];

    file1=argv[2];


    struct tms operation_time[5]; //user and system
    clock_t operation_time_real[5]; //real

    times(&operation_time[0]);
    operation_time_real[0] = clock();

    library(file1,c);
    
    times(&operation_time[1]);
    operation_time_real[1] = clock();

    system_func(file1,c);

    times(&operation_time[2]);
    operation_time_real[2] = clock();

    FILE *file = fopen("pomiar_zad_2.txt","w+");

    fprintf(file,"%s","\n              REAl        USER        SYSTEM\n");
    fprintf(file,"LIBRARY     %lf", calculate_time_clocks(operation_time_real[0], operation_time_real[1]));
    fprintf(file,"    %lf", calculate_time_tics(operation_time[0].tms_utime, operation_time[1].tms_utime));
    fprintf(file,"    %lf\n", calculate_time_tics(operation_time[0].tms_stime, operation_time[1].tms_stime));

    fprintf(file,"SYSTEM      %lf", calculate_time_clocks(operation_time_real[1], operation_time_real[2]));
    fprintf(file,"    %lf", calculate_time_tics(operation_time[1].tms_utime, operation_time[2].tms_utime));
    fprintf(file,"    %lf\n", calculate_time_tics(operation_time[1].tms_stime, operation_time[2].tms_stime));

    fclose(file);

    return 0;
}