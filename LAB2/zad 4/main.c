#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *file1=malloc(sizeof(char*)),*file2=malloc(sizeof(char*)),*row1=malloc(sizeof(char*)),*row2=malloc(sizeof(char*));
    ssize_t len=0;
    puts("Podaj nazwe pliku do odczytu: ");
    scanf("%s",file1);
    puts("Podaj nazwe pliku do zapisu: ");
    scanf("%s",file2);
    puts("Podaj pierwszy napis: ");
    scanf("%s1023[^\n]",row1);
    puts("Podaj drugi napis: ");
    scanf("%s1023[^\n]",row2);
    FILE* f=fopen(file1,"r+");
    FILE* output=fopen(file2,"w+");

    if(f==NULL){
        printf("\nError while loading file %s\n",file1);
        return 0;
    }

    char* text=malloc(sizeof(char*));

    while(fscanf(f,"%s",text)!=-1){
        if(!strcmp(text,row1)){
            text=row2;
        }
        printf("%s\n",text);
        fprintf(output,"%s\n",text);
    }
    return 0;

    fclose(f);
    fclose(output);
}