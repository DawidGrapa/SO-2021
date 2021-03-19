#define  _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>



#include "library.h"

void *dl_open;

struct Block *(*dl_create_main_block)();
struct Pair *(*dl_create_pairs)();
void (*dl_merge)();
int (*dl_create_block)();
int (*dl_rows_in_block)();
void (*dl_delete_block)();
void (*dl_delete_row_from_block)();
void (*dl_display)();


void run_tests(int blocks, int rows,char kind_of_test){

    struct Block *main_block = dl_create_main_block(blocks);
    struct Pair *pairs = dl_create_pairs(blocks);
    printf("%c",kind_of_test);
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

    struct tms tests_time[5];    //user and system
    clock_t tests_time_real[5];  //real

    times(&tests_time[0]);
    tests_time_real[0]=clock();

    dl_merge(pairs,blocks);  

    times(&tests_time[1]);
    tests_time_real[1]=clock();

    for(int i=0;i<blocks;i++){
        dl_create_block(main_block,i,pairs[i].merged_file,pairs[i].rows);
    }

    times(&tests_time[2]);
    tests_time_real[2]=clock();

    for(int i=0;i<blocks;i++){
        dl_delete_block(main_block,i);
    }

    times(&tests_time[3]);
    tests_time_real[3]=clock();

    for(int i = 0; i <= 5; i++) {
        for(int j = 0; j < blocks; j++) {
            dl_create_block(main_block, j, pairs[j].merged_file,  pairs[j].rows);
            dl_delete_block(main_block, j);
        }
    }
    times(&tests_time[4]);
    tests_time_real[4]=clock();

    puts("  REAL TIME:");
    printf("      FOR MERGING: %lf\n", (double) (tests_time_real[1] -tests_time_real[0]) / CLOCKS_PER_SEC);
    printf("      FOR CREATING BLOCKS: %lf\n", (double) (tests_time_real[2] -tests_time_real[1]) / CLOCKS_PER_SEC);
    printf("      FOR DELETING BLOCKS: %lf\n", (double) (tests_time_real[3] -tests_time_real[2]) / CLOCKS_PER_SEC);
    printf("      FOR CREATING AND DELETING BLOCKS: %lf\n\n", (double) (tests_time_real[4] -tests_time_real[3]) / CLOCKS_PER_SEC);

    puts("  USER TIME:");
    printf("      FOR MERGING: %lf\n", (double) (tests_time[1].tms_utime -tests_time[0].tms_utime) / sysconf(_SC_CLK_TCK));
    printf("      FOR CREATING BLOCKS: %lf\n", (double) (tests_time[1].tms_utime -tests_time[0].tms_utime) / sysconf(_SC_CLK_TCK));
    printf("      FOR DELETING BLOCKS: %lf\n", (double) (tests_time[1].tms_utime -tests_time[0].tms_utime) / sysconf(_SC_CLK_TCK));
    printf("      FOR CREATING AND DELETING BLOCKS: %lf\n", (double) (tests_time[1].tms_utime -tests_time[0].tms_utime) / sysconf(_SC_CLK_TCK));

    puts("  SYSTEM TIME:");
    printf("      FOR MERGING: %lf\n", (double) (tests_time[1].tms_stime -tests_time[0].tms_stime) / sysconf(_SC_CLK_TCK));
    printf("      FOR CREATING BLOCKS: %lf\n", (double) (tests_time[1].tms_stime -tests_time[0].tms_stime) / sysconf(_SC_CLK_TCK));
    printf("      FOR DELETING BLOCKS: %lf\n", (double) (tests_time[1].tms_stime -tests_time[0].tms_stime) / sysconf(_SC_CLK_TCK));
    printf("      FOR CREATING AND DELETING BLOCKS: %lf\n", (double) (tests_time[1].tms_stime -tests_time[0].tms_stime) / sysconf(_SC_CLK_TCK));
}

int main(int argc, char **argv){

    srand((unsigned int) time(NULL));

    dl_open = dlopen("./library_shared.so", RTLD_LAZY);  //z prezentacji Pana Majchera
    if(dl_open == NULL){
        printf("Cannot find library");
        return 1;
    }

    dl_create_main_block = dlsym(dl_open, "create_main_block");
    dl_create_pairs = dlsym(dl_open, "create_pairs");
    dl_merge = dlsym(dl_open, "merge");
    dl_create_block = dlsym(dl_open, "create_block");
    dl_rows_in_block = dlsym(dl_open, "rows_in_block");
    dl_delete_block = dlsym(dl_open, "delete_block");
    dl_delete_row_from_block = dlsym(dl_open, "delete_row_from_block");
    dl_display = dlsym(dl_open, "display");

    if(!strcmp(argv[1],"Tests")){  //returns 1 if testing
        puts("\nSmall tests - 5 blocks 10 rows\n");
        run_tests(5,10,'S');
        puts("\nMedium Tests - 100 blocks 50 rows\n");
        run_tests(20,50,'M');
        puts("\nLarge tests - 1000 blocks - 500 rows\n");
        run_tests(1000,500,'L');
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

    struct Block * main_block=dl_create_main_block(blocks);

    struct Pair * pairs = dl_create_pairs(blocks);

    int argument = 4;  // argv[1,2,3] -> creating and merging. argv[4] -> files
    int index = 0;  //value to know where is :
    
    for(int i=0;i<blocks;i++){
        char *pair = argv[argument];
        index = (int) (strchr(pair,':')-pair);  

        pairs[i].first_file = calloc((index+1),sizeof(char));   //\0
        pairs[i].second_file = calloc((strlen(pair)-index+1), sizeof(char));  //\0
        
        sscanf(pair, "%[^:]:%s", pairs[i].first_file, pairs[i].second_file); 

        argument++;
    }
    
    dl_merge(pairs,blocks);

    for(int k=0;k<blocks;k++){
        dl_create_block(main_block,k,pairs[k].merged_file,pairs[k].rows);
    }
    

    while(argument<argc){
        char* command = argv[argument];
            
        if(!strcmp(command,"remove_block")){
            argument++;
                index=strtol(argv[argument],NULL,10);
                if(index<blocks && index>=0){
                    dl_delete_block(main_block,index);
                }   
            
        } 
        else if(!strcmp(command,"remove_row")){
            argument++;
            int block_index = strtol(argv[argument],NULL,10);
            argument++;
            int row_index = strtol(argv[argument],NULL,10);

            if((block_index>=0 && block_index<blocks) && (row_index>=0 && row_index<dl_rows_in_block(main_block,block_index))){
                dl_delete_row_from_block(main_block,block_index,row_index);
            }
            
        }
        else{
            printf("\n%s is wrong command!\n",command);
            break;
        }
        argument++;
                
    }
    
    printf("\n\n");
    dl_display(main_block,blocks);
    printf("\n\n");

    
    return 0;
}