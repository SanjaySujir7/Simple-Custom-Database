#ifndef MAIN_H
#define MAIN_H

/*
    Resources means the core header for lots of function.
*/


// Will save lot of space :D
typedef unsigned char U_Char;
typedef unsigned short U_Short;


typedef struct Header_Meta{
    U_Short Total_Pages;
    U_Char Total_Tables;
    U_Short Next_Head;
    
} Page_Meta;


#endif