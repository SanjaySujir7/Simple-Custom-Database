#include "Resource.h"
#include<string.h>

/*
    This file containes code related to the table opertion. 
    Whole file is documented for me for future since reading C code is hard.
*/

char Error_Buffer[10];
extern Table_Cache *T_Cache;


// This function is for creatign the table. Error Codes =[1,3,4,5,6]
U_Char Create_Table(const char *Database_Name, char Table_Name[],Column_Data *Column_Struct, const U_Char Col_Size){
    FILE *file = fopen(Database_Name,"rb+");

    if(file == NULL){
        return 1;
    }

    char Buffer[TOTAL_META_SIZE];

    if(fread(Buffer,1,TOTAL_META_SIZE,file) != TOTAL_META_SIZE){
        fclose(file);
        return 1;
    }

    U_Short Bytes_Count = 0,Size;
    // Page_Meta Meta;

    Size = sizeof(Page_Meta);

    Page_Meta *Meta = (Page_Meta *)Buffer;

    Bytes_Count += Size;

    // This is crucial other wise Data wil overflow from Meta and can overwrite other data.
    if(Meta->Total_Tables >= 5){
        fclose(file);
        return 5;
    }

    if(Meta->Total_Tables == 0){

        Table_Meta  Table_Array[5];
        Column_Data Col_Array[5][15];
        Table_Meta Table;

        // Copying The Table Name To The Table Struct. May have been directly intialized but i chose this way.
        strncpy(Table.Table_Name, Table_Name, sizeof(Table.Table_Name) - 1);  
        
        Table.Table_Name[sizeof(Table.Table_Name) - 1] = '\0';  
        Table.Total_Columns = Col_Size;

        Table_Array[0] = Table;

        for(U_Char i = 0; i < Col_Size; i++){
            Col_Array[0][i] = Column_Struct[i];
        }

        Meta->Total_Tables += 1;

        // Copying raw memory to struct after processing Data.
        Size = sizeof(Table_Array);
        memcpy(Buffer + Bytes_Count,Table_Array,Size);
    
        Bytes_Count += Size;

        Size = (sizeof(Column_Data) * 15) * 5;
        memcpy(Buffer + Bytes_Count,Col_Array,Size);
    
        Bytes_Count += Size;

        // Seek to start of the file. and Writing whole buffer instead of one by one. it makes process clear and efficient.
        fseek(file,0,SEEK_SET);
        Bytes_Count = fwrite(Buffer,1,TOTAL_META_SIZE,file);

        if(Bytes_Count != TOTAL_META_SIZE){
            fclose(file);
            return 3;
        }
    }
    else{
        // This runs if atleast one table alredy exists in the Database.
        // Here i chose to copy raw memory from Buffer to Struct instead of casting. This makes clear. Less Bugs:D
        Table_Meta Table_Array[5];
        memcpy(Table_Array,Buffer + Bytes_Count,sizeof(Table_Array));
        
        // For checking if the Table already exists.
        for(U_Char i = 0; i < Meta->Total_Tables;i++){
            if(strcmp(Table_Name,Table_Array[i].Table_Name) == 0){
                fclose(file);
                return 6;
            }
        }

        Table_Meta Table;
        
        strncpy(Table.Table_Name,Table_Name,sizeof(Table.Table_Name) - 1);
        Table.Table_Name[sizeof(Table.Table_Name) - 1] = '\0';

        Table.Total_Columns = Col_Size;
        Table.Table_Head = 0;

        Table_Array[Meta->Total_Tables] = Table;

        // Copying Memory back to Buffer after processing.
        Size = sizeof(Table_Array);
        memcpy(Buffer + Bytes_Count,Table_Array,Size);
        Bytes_Count += Size;


        Column_Data Column_Array[5][15];

        memcpy(Column_Array,Buffer + Bytes_Count,sizeof(Column_Array));

        for(U_Char i = 0; i < Col_Size; i++){
            Column_Array[Meta->Total_Tables][i] = Column_Struct[i];
        }

        Meta->Total_Tables += 1;

        memcpy(Buffer + Bytes_Count,Column_Array,sizeof(Column_Array));
        
        // seek to the begining since we are writing whole buffer
        fseek(file,0,SEEK_SET);
    
        Bytes_Count = fwrite(Buffer,1,TOTAL_META_SIZE,file);

        if(Bytes_Count != TOTAL_META_SIZE){
            fclose(file);

            return 4;
        }
    }

    fclose(file);

    return 0;
}


// Function to get details about table Error =[TisNULL,TNF]
Table_Cache *Request_Table_Info(FILE *file,char *Table_Name){

    if(file == NULL){
        return NULL;
    }

    char Buffer[TOTAL_META_SIZE];
    U_Short Bytes_Count = 0,Size = 0;

    static Table_Cache Cache_Table = {0,"NULL",0,0,0};
    
    Bytes_Count = fread(Buffer,1,TOTAL_META_SIZE,file);

    if(Bytes_Count != TOTAL_META_SIZE){
        return NULL;
    }

    Page_Meta *Meta;
    Table_Meta Table_Array[5];
    Column_Data Coulmn_Array[5][15];

    Meta = (Page_Meta*) Buffer;
    Bytes_Count = sizeof(Page_Meta);

    // Debuging
    printf("The total Tables are : %d",Meta->Total_Tables);

    if(Meta->Total_Tables == 0){
        strcpy(Error_Buffer,"TisNULL");
        return &Cache_Table;
    }

    Size = sizeof(Table_Array);
    memcpy(Table_Array,Buffer + Bytes_Count,Size);
    Bytes_Count += Size;

    Size = sizeof(Coulmn_Array);
    memcpy(Coulmn_Array,Buffer + Bytes_Count,Size);
    Bytes_Count += Size;

    printf("\nTotal Extracted from buffer = %d",Bytes_Count);

    U_Char i,Table_Index = 50;

    // To check if the Table exits or not. if exists it will copy the data.
    for(i = 0; i < Meta->Total_Tables;i++){
        if(strcmp(Table_Name,Table_Array[i].Table_Name) == 0){
            Cache_Table.Cache_Exits = 1;

            strncpy(Cache_Table.Table_Name,Table_Array[i].Table_Name,sizeof(Cache_Table.Table_Name) - 1);

            Cache_Table.Table_Head = Table_Array[i].Table_Head;
            Cache_Table.Total_Columns = Table_Array[i].Total_Columns;

            Table_Index = i;
            break;
        }
    }

    if(Table_Index == 50){
        strcpy(Error_Buffer,"TNF");
        return NULL;
    }

    // Copying the Column Data to the cache.
    memcpy(Cache_Table.Table_Columns,Coulmn_Array[Table_Index],sizeof(Column_Data) * 15);

    // debug
    printf("\nCoulmn_Name = %s",Cache_Table.Table_Columns[2].Column_Name);

    return &Cache_Table;
}
