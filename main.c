#include "Resource.h"


/*
    This is file contain code for connecting other module. Whole project is documented. added comments so it makes easy for me to read in future.
    A Simple Database which may have CRUD operations (Not sure).
    No binary tree or Hash Tables whatever beacuse iam too lazy to add that.
*/


// Function proto type that exists in other module
extern U_Char Create_Database_Meta(FILE *file);
extern U_Char Create_Table(const char *Database_Name, const char Table_Name[],Column_Data *Column_Struct, const U_Char Col_Size);
extern Table_Cache *Request_Table_Info(FILE *file,char *Table_Name);

extern char Error_Buffer[10];

// Pointer vaible for cache
Table_Cache *T_Cache = NULL;



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

void Handle_Table_Command(){
    U_Char Return;

    Column_Data Array[15];
    Column_Data Column1 = {"Name","String",50};
    Column_Data Column2 = {"Age","Number",0};
    Column_Data Column3 = {"Roll_NO","Number",10};

    Array[0] = Column1;
    Array[1] = Column2;
    Array[2] = Column3;

    Return = Create_Table("my.cdb","Student",Array,3);

    if(Return == 6){
        printf("\nThe Table is alredy exists.");
    }

    else if(Return != 0){
        printf("Something Gone Wrong!");
    }
   
    return;
}

void Handle_Insertion(){
    FILE *file = fopen("my.cdb","rb");
    if(T_Cache == NULL){
        Table_Cache *Cache = Request_Table_Info(file,"Student");

        if(Cache == NULL){
            printf("Error is = %s",Error_Buffer);
        }
        else{
            T_Cache = Cache;
        }
    }

    

    fclose(file);
    return ;
}



int main(){
    char Return;

    // Return = Create_Database("my.cdb");

    // if(Return == 1){
    //     printf("Creation Failed!");
    // }
    
    Handle_Insertion();

    return 0;
}