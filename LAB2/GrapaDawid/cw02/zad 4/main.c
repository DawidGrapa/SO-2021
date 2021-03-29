#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <time.h>
#include <math.h>

double calculate_time_clocks(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

double calculate_time_tics(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void system_func(char *file1,char *file2,char*row1,char*row2){
    int f=open(file1,O_RDONLY);
    int output=open(file2,O_CREAT|O_RDWR|O_TRUNC, 0666);

    if(f==-1){
        printf("\nError while loading file %s\n",file1);
        return;
    }
    char c;
    char* text=malloc(sizeof(char)*strlen(row1)+1);

    while(read(f,&c,1)) {
        if(c!=row1[0]){
            write(output,&c,1);
        }
        else{
            lseek(f,-1,SEEK_CUR);
            read(f,text,strlen(row1));
            if(!strcmp(row1,text)){
                write(output,row2,strlen(row2));
            }
            else{
                write(output,text,strlen(text));
            }
        }
    }
    close(f);
    close(output);
}

void library(char* file1,char*file2,char*row1,char*row2){

    FILE* f=fopen(file1,"r+");
    FILE* output=fopen(file2,"w+");


    if(f==NULL){
        printf("\nError while loading file %s\n",file1);
        return;
    }
    char c;
    char* text=malloc(sizeof(char)*strlen(row1)+1);

    while(fread(&c,sizeof(char),1,f)){
        if(c!=row1[0]){
            fwrite(&c,sizeof(char),1,output);
        }
        else{
            fseek(f,-1,1);   //trzeba sie cofnac, poniewaz po powyzszym fread pozycja jest o jeden w prawo
            fread(text,sizeof(char),strlen(row1),f);
            if(!strcmp(row1,text)){  //jezeli te same napisy, to podmieniam
                fwrite(row2,sizeof(char),strlen(row2),output);
            }
            else{  //jesli nie no to nie
                fwrite(text,sizeof(char),strlen(text),output);
            }
        }
    }


    fclose(f);
    fclose(output);
}

int main(int argc,char *argv[]){

    if(argc<5){
        puts("Not enough arguments");
        return 0;
    }

    struct tms operation_time[5]; //user and system
    clock_t operation_time_real[5]; //real

    times(&operation_time[0]);
    operation_time_real[0] = clock();
    
    library(argv[1],argv[2],argv[3],argv[4]);

    times(&operation_time[1]);
    operation_time_real[1] = clock();

    system_func(argv[1],argv[2],argv[3],argv[4]);

    times(&operation_time[2]);
    operation_time_real[2] = clock();

    FILE *file = fopen("pomiar_zad_4.txt","w+");

    fprintf(file,"%s","\n              REAl        USER        SYSTEM\n");
    fprintf(file,"LIBRARY     %lf", calculate_time_clocks(operation_time_real[0], operation_time_real[1]));
    fprintf(file,"    %lf", calculate_time_tics(operation_time[0].tms_utime, operation_time[1].tms_utime));
    fprintf(file,"    %lf\n", calculate_time_tics(operation_time[0].tms_stime, operation_time[1].tms_stime));

    fprintf(file,"SYSTEM      %lf", calculate_time_clocks(operation_time_real[1], operation_time_real[2]));
    fprintf(file,"    %lf", calculate_time_tics(operation_time[1].tms_utime, operation_time[2].tms_utime));
    fprintf(file,"    %lf\n", calculate_time_tics(operation_time[1].tms_stime, operation_time[2].tms_stime));

    fclose(file);

}