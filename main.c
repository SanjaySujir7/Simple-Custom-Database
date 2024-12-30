#include "Main.h"
#include<stdio.h>
#include "Resource.h"


// Function proto type that exists in other module
extern U_Char Create_Database_Meta(FILE *file);
extern U_Char Create_Table(const char *Database_Name, const char Table_Name[],Column_Data *Column_Struct, const U_Char Col_Size);


char Create_Database(char *Datbase_Name){
    // Create the Database
    FILE *file = fopen(Datbase_Name,"wb");

    if(file == NULL){
        return 1;
    }

    if(Create_Database_Meta(file) != 0){
        fclose(file);

        printf("Error Creating data base !.");
        return 1;
    }

    fclose(file);    
    return 0;
}




int main(){
    char Return;

    // Return = Create_Database("my.cdb");

    // if(Return == 1){
    //     printf("Creation Failed!");
    // }
    

    Column_Data Array[15];
    Column_Data Column1 = {"Name","String",50};
    Column_Data Column2 = {"Age","Number",0};
    Column_Data Column3 = {"Roll_NO","Number",10};

    Array[0] = Column1;
    Array[1] = Column2;
    Array[2] = Column3;

    Return = Create_Table("my.cdb","Student",Array,3);

    if(Return != 0){
        printf("Something Gone Wrong!");
    }

    return 0;
}