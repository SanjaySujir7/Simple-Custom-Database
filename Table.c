#include "Resource.h"
#include<string.h>

/*
    This file containes code related to the table opertion
*/

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

    if(Meta->Total_Tables == 0){
        // Debuging
        printf("Meta Total_Table = %d",Meta->Total_Tables);
        Table_Meta  Table_Array[5];
        Column_Data Col_Array[5][15];

        Table_Meta Table;
        strncpy(Table.Table_Name, Table_Name, sizeof(Table.Table_Name) - 1);  
        
        Table.Table_Name[sizeof(Table.Table_Name) - 1] = '\0';  
        Table.Total_Columns = Col_Size;

        Table_Array[0] = Table;

        for(U_Char i = 0; i < Col_Size; i++){
            Col_Array[0][i] = Column_Struct[i];
        }

        Meta->Total_Tables += 1;

        Size = sizeof(Table_Array);
        memcpy(Buffer + Bytes_Count,Table_Array,Size);
        printf("\nWriting Table array in = %d",Bytes_Count);
        Bytes_Count += Size;

        Size = (sizeof(Column_Data) * 15) * 5;
        memcpy(Buffer + Bytes_Count,Col_Array,Size);
        printf("\nWriting Col array in = %d",Bytes_Count);
        Bytes_Count += Size;

        printf("\nTotal Size =  %d",Bytes_Count);

        // Seek to start 
        fseek(file,0,SEEK_SET);
        Bytes_Count = fwrite(Buffer,1,TOTAL_META_SIZE,file);

        printf("\nThe Total Bytes written is %d ",Bytes_Count);
        if(Bytes_Count != TOTAL_META_SIZE){
            fclose(file);
            return 3;
        }
    }

    fclose(file);

    return 0;
}