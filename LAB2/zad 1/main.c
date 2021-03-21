#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <time.h>

const size_t BUFFOR = 1000;

double calculate_time_clocks(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

double calculate_time_tics(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void system_func(const char *file1,const char*file2){
    
    int f1=open(file1,O_RDONLY);
    int f2=open(file2,O_RDONLY);
    
    if(f1==-1 || f2==-1){
        puts("Error while reading files.");
        return;
    }


    char *text1 = malloc(sizeof(char)*BUFFOR);
    char *text2 = malloc(sizeof(char)*BUFFOR);
    
    

    printf("\nSYSTEM\n\n");

    size_t size1 = read(f1,text1,BUFFOR);
    size_t size2 = read(f2,text2,BUFFOR);

    size_t temp1=0,temp2=0;

    while(size1 || size2){  //literka po literce, dopoki nie ma nowej linii albo jest co czytac
        bool notslashn1=true;
        while(size1 && notslashn1){
            write(1,&text1[temp1],1);
            if(text1[temp1]=='\n'){
                notslashn1=false;
            }
            temp1++;
            if(temp1>=size1){
                size1=read(f1,text1,BUFFOR);
                temp1=0;
            }
        }
        bool notslashn2=true;
        while(size2 && notslashn2){
            write(1,&text2[temp2],1);
            if(text2[temp2]=='\n'){
                notslashn2=false;
            }
            temp2++;
            if(temp2>=size2){
                size2=read(f2,text2,BUFFOR);
                temp2=0;
            }
            
        }
        
        
    }

    close(f1);
    close(f2);
    free(text1);
    free(text2);
}

void library(const char *file1,const char*file2){

    FILE* f1=fopen(file1,"r");
    FILE* f2=fopen(file2,"r");

    if(f1==NULL || f2==NULL){
        puts("Error while reading files.");
        return;
    }


    char *text1 = malloc(sizeof(char)*BUFFOR);
    char *text2 = malloc(sizeof(char)*BUFFOR);
    
    

    printf("LIBRARY\n\n");

    size_t size1 = fread(text1,sizeof(char),BUFFOR,f1);
    size_t size2 = fread(text2,sizeof(char),BUFFOR,f2);

    size_t temp1=0,temp2=0;

    while(size1 || size2){
        bool notslashn1=true;
        while(size1 && notslashn1){
            fwrite(&text1[temp1],sizeof(char),1,stdout);
            if(text1[temp1]=='\n'){
                notslashn1=false;
            }
            temp1++;
            if(temp1>=size1){
                size1=fread(text1,sizeof(char),BUFFOR,f1);
                temp1=0;
            }
        }
        bool notslashn2=true;
        while(size2 && notslashn2){
            fwrite(&text2[temp2],sizeof(char),1,stdout);
            if(text2[temp2]=='\n'){
                notslashn2=false;
            }
            temp2++;
            if(temp2>=size2){
                size2=fread(text2,sizeof(char),BUFFOR,f2);
                temp2=0;
            }
            
        }
        
        
    }

    fclose(f1);
    fclose(f2);
    free(text1);
    free(text2);
}


int main(int argc,char **argv){

    char *file1=malloc(sizeof (char*)),*file2=malloc(sizeof (char*));

    

    if(argc>2){
        file1=argv[1];
        file2=argv[2];
    }
    else if(argc == 2){
        file1=argv[1];
        printf("Podaj nazwe drugiego pliku: ");
        scanf("%s",file2);
    }   
    else{
        printf("Podaj nazwy plikow po spacji: ");
        scanf("%s",file1);
        scanf("%s",file2);
    }
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

    FILE *file = fopen("pomiar_zad_1.txt","w+");

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
