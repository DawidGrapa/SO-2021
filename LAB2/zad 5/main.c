#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <time.h>


double calculate_time_clocks(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

double calculate_time_tics(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void system_func(char *file1, char* file2){
    int f1=open(file1,O_RDONLY);
    int f2=open(file2,O_CREAT|O_RDWR|O_TRUNC, 0666);

    if(f1==-1){
        printf("Error while loading file.");
        return;
    }

    size_t temp=0;
    char slashn='\n';
    char text;

    while(read(f1,&text,1)){
        if(temp<50){
            write(f2,&text,1);
            temp++;
            if(text=='\n'){
                temp=0;
            }
        }
        else{
            write(f2,&slashn,1);
            write(f2,&text,1);
            temp=1;
        }
    }

    close(f1);
    close(f2);
}

void library(char *file1,char*file2){
    FILE* f1=fopen(file1,"r+");
    FILE* f2=fopen(file2,"w+");

    if(f1==NULL){
        printf("Error while loading file.");
    }
    char slashn='\n';
    char text;



    size_t temp=0;

    while(fread(&text,sizeof(char),1,f1)){
        if(temp<50){
            fwrite(&text,sizeof(char),1,f2);
            temp++;
            if(text=='\n'){
                temp=0;
            }
        }
        else{
            fwrite(&slashn,sizeof(char),1,f2);
            fwrite(&text,sizeof(char),1,f2);
            temp=1;
        }
    }
    fclose(f1);
    fclose(f2);
}
int main(int argc,char **argv){

    char *file1=malloc(100),*file2=malloc(100);

    if(argc<3){
        puts("Not enough arguments.");
        return 0;
    }

    file1=argv[1];
    file2=argv[2];

    struct tms operation_time[5]; //user and system
    clock_t operation_time_real[5]; //real

    times(&operation_time[0]);
    operation_time_real[0] = clock();

    library(file1,file2);
    
    times(&operation_time[1]);
    operation_time_real[1] = clock();

    system_func(file1,file2);

    times(&operation_time[2]);
    operation_time_real[2] = clock();

    FILE *file = fopen("pomiar_zad_5.txt","w+");

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