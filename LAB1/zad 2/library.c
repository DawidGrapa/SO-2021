#define  _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "library.h" //my own library



//Functions

struct Block *create_main_block(int size){
    if(size<0) return NULL;
    struct Block *MainBlock = calloc(size, sizeof(struct Block)); //array of size 'size'
    return MainBlock;
}

struct Pair *create_pairs(int size){
    if(size<0) return NULL;
    struct Pair* pairs = calloc(size, sizeof(struct Pair));

    return pairs;
}

void merge(struct Pair *pairs,int size){
    if(size<0) return;

    for(int i=0;i<size;i++){
        FILE * first = fopen(pairs[i].first_file, "r");  //r - read
        FILE * second = fopen(pairs[i].second_file, "r");
    
        if(first==NULL || second==NULL){
            printf("ERROR WHILE READING pair: %s:%s\n", pairs[i].first_file, pairs[i].second_file);
            return;
        }
        char file_name[50]="Merged";
        char number[30];
        

        sprintf(number,"%d",i); //converts int to string
        strcat(file_name,number);
        strcat(file_name, ".txt");

        pairs[i].merged_file=malloc(strlen(file_name) * sizeof(char));

        FILE *f = fopen(file_name, "w+"); //open or create a new file

        strcpy(pairs[i].merged_file, file_name);  //copy file name to pairs
        int a=1,b=1;
        char* text;       
        int rows=0; 
        size_t len=0;

        while(a || b){
            if(a) {
                if(getline(&text,&len, first) != -1) {
                    fprintf(f, "%s", text);
                    rows++;
                } else {
                    a = 0;
                }
            }
            if(b) {
                if(getline(&text,&len, second) != -1) {
                    fprintf(f, "%s", text);
                    rows++;
                } else {
                    b = 0;
                }
            }
        }

        pairs[i].rows=rows;
        fclose(first);
        fclose(second);
        fclose(f);
        
    }   
}

int create_block(struct Block *main_block,int index,char *file,int num_rows){
    size_t len;

    FILE *f = fopen(file,"r");

    char **rows = calloc(num_rows, sizeof(char*));

    for(int i=0; i<num_rows;i++){
        getline(&rows[i], &len, f);
    }

    main_block[index].size=num_rows;
    main_block[index].rows=rows;
    return index;
}

int rows_in_block(struct Block *block, int index){
    return block[index].size;
}

void delete_block(struct Block *main_block,int index){
    for(int i = 0; i < main_block[index].size; i++) {
        free(main_block[index].rows[i]);
        main_block[index].rows[i]=NULL;
    }
    free(main_block[index].rows);
    main_block[index].rows=NULL;
}

void delete_row_from_block(struct Block *main_block,int block_index,int row_index){
    free(main_block[block_index].rows[row_index]);
    main_block[block_index].rows[row_index]=NULL;
}

void display(struct Block *main_block,int no_blocks){
    for(int i=0;i<no_blocks;i++){
        printf("BLOCK NO %d \n",i);
        if(main_block[i].rows==NULL){
            printf("This block was deleted!\n");
        }
        else{
            for(int j=0;j<main_block[i].size;j++){
                if(main_block[i].rows[j]==NULL){
                    printf("This row was deleted!\n");
                }
                else{
                    printf("%s",main_block[i].rows[j]);
                }
            }
        }
        printf("\n\n");
    }
    
}





