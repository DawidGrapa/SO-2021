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

const size_t BUFFOR = 257;

double calculate_time_clocks(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

double calculate_time_tics(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void system_func(){
    int dane = open("dane.txt",O_RDONLY);
    if(dane==-1){
        puts("Error while opening dane.txt.");
        return;
    }
    int a=open("a_system.txt",O_CREAT|O_RDWR|O_TRUNC, 0666);
    int b=open("b_system.txt",O_CREAT|O_RDWR|O_TRUNC, 0666);
    int c=open("c_system.txt",O_CREAT|O_RDWR|O_TRUNC, 0666);

    float root;
    int j;
    int parzyste=0;

    char *text=malloc(BUFFOR*sizeof(char));

    size_t len=read(dane,text,BUFFOR);

    size_t temp=0;
    printf("\nSYSTEM\n\n");
    char number[30];

    while(len){
        while(text[temp]!='\n'){
            temp++;
        }
        int a = atoi(text);
        j=a;
        if(!((int)a%2)){
            parzyste++;
        }
        if((j%100)/10==7 || (j%100)/10==0){
            write(b,text,temp+1);
        }
        root=sqrt((double)a);
        j=root;
        if(j==root){
            sprintf(number,"%d",j*j);
            strcat(number,"\n");
            write(c,number,strlen(number));
        }
        lseek(dane,temp-len+1,SEEK_CUR); //temp-value of first sign of new line - len - value of last get char value -> cofa mnie do miejsca w ktorym jest temp
        len=read(dane,text,BUFFOR);
        temp=0;
    }

   
    char help[50] = "Liczb parzystych jest: ";
    sprintf(number,"%d",parzyste);
    strcat(help,number);
    write(a,help,strlen(help));

    close(dane);
    close(a);
    close(b);
    close(c);
    free(text);
}

void library(){
    FILE *dane = fopen("dane.txt","r+");
    if(dane==NULL){
        puts("Error while opening dane.txt.");
        return;
    }
    FILE *a=fopen("a.txt","w+");
    FILE *b=fopen("b.txt","w+");
    FILE *c=fopen("c.txt","w+");

    float root;
    int j;
    int parzyste=0;
    printf("LIBRARY\n\n");
    char *text=malloc(BUFFOR*sizeof(char));

    size_t len=fread(text,sizeof(char),BUFFOR,dane);

    size_t temp=0;

    char number[30];

    while(len){
        while(text[temp]!='\n'){
            temp++;
        }
        int a = atoi(text);
        j=a;
        if(!((int)a%2)){
            parzyste++;
        }
        if((j%100)/10==7 || (j%100)/10==0){
            fwrite(text,sizeof(char),temp+1,b);
        }
        root=sqrt((double)a);
        j=root;
        if(j==root){
            sprintf(number,"%d",j*j);
            strcat(number,"\n");
            fwrite(number,sizeof(char),strlen(number),c);
        }
        fseek(dane,temp-len+1,1); //temp-value of first sign of new line - len - value of last get char value -> cofa mnie do miejsca w ktorym jest temp
        len=fread(text,sizeof(char),BUFFOR,dane);
        temp=0;
    }

   
    char help[50] = "Liczb parzystych jest: ";
    sprintf(number,"%d",parzyste);
    strcat(help,number);
    fwrite(help,sizeof(char),strlen(help),a);

    fclose(dane);
    fclose(a);
    fclose(b);
    fclose(c);
    free(text);
}

int main(){
    struct tms operation_time[5]; //user and system
    clock_t operation_time_real[5]; //real

    times(&operation_time[0]);
    operation_time_real[0] = clock();

    library();
    
    times(&operation_time[1]);
    operation_time_real[1] = clock();

    system_func();

    times(&operation_time[2]);
    operation_time_real[2] = clock();

    FILE *file = fopen("pomiar_zad_3.txt","w+");

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