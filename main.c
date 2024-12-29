#include<stdio.h>
#include "Main.h"



char Create_Database(char *Datbase_Name){
    // Create the Database
    FILE *file = fopen(Datbase_Name,"wb");

    if(file == NULL){
        return 1;
    }


    fclose(file);    
    return 0;
}


int main(){
    char Return;

    Return = Create_Database("my.cdb");

    if(Return == 1){
        printf("Creation Failed!");
    }

    return 0;
}