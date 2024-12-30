#include "Resource.h"
#include<string.h>

/*
    This file containes code related to the table opertion. 
    Whole file is documented for me for future since reading C code is hard.
*/

// This function is for creatign the table. Error Codes =[1,3,4,5]
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