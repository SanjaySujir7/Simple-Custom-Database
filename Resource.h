#ifndef RESOURCE_H
#define RESOURCE_H

#include<stdio.h>

/*
    Resources means memory in this program. and this header is core header for entire programm.
*/

// Definations 
#define TOTAL_META_SIZE 3146

// Will save lot of space :D
typedef unsigned char U_Char;
typedef unsigned short U_Short;


// This is the meta that holds database info
typedef struct Header_Meta{
    U_Short Total_Pages;
    U_Char Total_Tables;
    U_Short Next_Head;
    
} Page_Meta;


// The meta data for the Table.
typedef struct {
    char Table_Name[25];
    U_Char Total_Columns;
    U_Short Table_Head;

} Table_Meta;


typedef struct {
    char Column_Name[25];
    char Type[10];
    unsigned int  Size;

} Column_Data;


// This is cache Table will hold data to do operations.
typedef struct {
    U_Char Cache_Exits;
    char Table_Name[25];
    U_Char Total_Columns;
    U_Char Table_Head;
    Column_Data Table_Columns[15];

} Table_Cache;


// Struct and Union combined to hold complicated data.
// The minimum value of The string is 25 if it exceeds we should Dynamically allocate.
typedef struct {

    enum Type{
        Integer,
        String
    } type;

    union Value{
        int Int;
        char String[25];
    } value;

} Data;

#endif