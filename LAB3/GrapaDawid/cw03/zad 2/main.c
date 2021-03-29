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
#include "library.h"

double calculate_time_clocks(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

double calculate_time_tics(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}


void run_tests(int blocks,char kind_of_test){

    struct Pair *pairs = create_pairs(blocks);
    for(int i=0;i<blocks;i++){
        if(kind_of_test=='S'){
            pairs[i].first_file=calloc(strlen("SMALL.txt")+1,sizeof(char));
            pairs[i].second_file = calloc(strlen("SMALL.txt")+1,sizeof(char));
            strcpy(pairs[i].first_file,"SMALL.txt");
            strcpy(pairs[i].second_file,"SMALL.txt");
        }
        if(kind_of_test=='M'){
            pairs[i].first_file=calloc(strlen("MEDIUM.txt")+1,sizeof(char));
            pairs[i].second_file = calloc(strlen("MEDIUM.txt")+1,sizeof(char));
            strcpy(pairs[i].first_file,"MEDIUM.txt");
            strcpy(pairs[i].second_file,"MEDIUM.txt");
        }
        if(kind_of_test=='L'){
            pairs[i].first_file=calloc(strlen("LARGE.txt")+1,sizeof(char));
            pairs[i].second_file = calloc(strlen("LARGE.txt")+1,sizeof(char));
            strcpy(pairs[i].first_file,"LARGE.txt");
            strcpy(pairs[i].second_file,"LARGE.txt");
        }
        
    }
    fflush(stdout);
    struct tms tests_time[2];    //user and system
    clock_t tests_time_real[2];  //real

    times(&tests_time[0]);
    tests_time_real[0]=clock();

    for(int i = 0; i < blocks; i++) {
        if(fork() == 0) {
            merge(pairs, i);
            exit(0);
        }
    }

    for(int i = 0; i < blocks; i++) {
        wait(NULL);
    } 

    times(&tests_time[1]);
    tests_time_real[1]=clock();
    

    puts("              REAl        USER        SYSTEM");
    printf("MERGE       %lf", calculate_time_clocks(tests_time_real[0], tests_time_real[1]));
    printf("    %lf", calculate_time_tics(tests_time[0].tms_utime, tests_time[1].tms_utime));
    printf("    %lf\n", calculate_time_tics(tests_time[0].tms_stime, tests_time[1].tms_stime));

    

}

int main(int argc, char **argv){

    if(argc<2) return 0;

    if(!strcmp(argv[1],"Tests")){  //returns 1 if testing
        puts("\nSmall tests - 5 blocks 10 rows\n");
        run_tests(5,'S');
        puts("\nMedium Tests - 200 blocks 100 rows\n");
        run_tests(200,'M');
        puts("\nLarge tests - 500 blocks - 1000 rows\n");
        run_tests(500,'L');
        return 0;
    }

    if(argc<5){  //argv[0] - program name like ./Zad2
        puts("Not enough arguments");
        return 1;
    }
    else if(strcmp(argv[1],"create_table")){
        puts("Wrong order. Create table first.");
        return 1;
    }
    else if(strcmp(argv[3],"merge_files")){ //returns 0 if strings are the same 
        puts("Wrong Order. You have to merge files.");
        return 1;
    }

    int blocks=strtol(argv[2],NULL,10);  // Null - end pointer - not used, 10 - numerical system, converts string to long int

    struct Pair * pairs = create_pairs(blocks);

    int argument = 4;  // argv[1,2,3] -> creating and merging. argv[4] -> files
    int index = 0;  //value to know where is :
    
    for(int i=0;i<blocks;i++){
        char *pair = argv[argument];
        index = (int) (strchr(pair,':')-pair);  //position of ':'

        pairs[i].first_file = calloc((index+1),sizeof(char));   // index + \0 
        pairs[i].second_file = calloc((strlen(pair)-index+1), sizeof(char));  //\0
        
        sscanf(pair, "%[^:]:%s", pairs[i].first_file, pairs[i].second_file); 

        argument++;
    }

    for(int i = 0; i < blocks; i++) {
        if(fork() == 0) {
            merge(pairs, i);
            return 0;
        }
    }
    
    for(int i = 0; i < blocks; i++) {
        wait(NULL);
    }
    

    return 0;
}