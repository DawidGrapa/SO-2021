//Structures I will use
#ifndef _LIBRARY_H
#define _LIBRARY_H
struct Block{
    int size;
    char **rows;
};

struct Pair{
    int rows;
    char* first_file;
    char* second_file;
    char* merged_file;
};

//Functions 
struct Block *create_main_block(int size);

struct Pair * create_pairs(int size);

void merge(struct Pair *pairs,int size);

int rows_in_block(struct Block *block, int index);

int create_block(struct Block *main_block,int index,char *file,int num_rows);

void delete_block(struct Block *main_block,int index);

void delete_row_from_block(struct Block *main_block,int block_index,int row_index);

void display(struct Block *main_block,int no_blocks);

#endif