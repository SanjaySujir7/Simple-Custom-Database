#include "Resource.h"
#include<string.h>


// Function that executes Paging. this is executed when Datbase is created.
U_Char Create_Database_Meta(FILE *file){
    if(file == NULL){
        return 1;
    }

    Page_Meta Meta;

    Meta.Next_Head = 0;
    Meta.Total_Pages = 0;
    Meta.Total_Tables = 0;

    char Buffer[TOTAL_META_SIZE];
    
    // for buffering the meta
    memcpy(Buffer,&Meta,sizeof(Page_Meta));

    // will check if the total size wriiten is not valid
    if(fwrite(Buffer,1,TOTAL_META_SIZE,file) != TOTAL_META_SIZE){
        return 2;
    }

    return 0;

}