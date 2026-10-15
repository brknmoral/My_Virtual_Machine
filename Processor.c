#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include <errno.h>
#include <sys/stat.h>


#define DATA_SIZE (sizeof(data)/sizeof(data[0]))

int ADD_ELEM(int a, int b)
{
    return (a + b);
}

int SUB_ELEM(int a, int b)
{
    return (a - b);
}

int DIV_ELEM(int a , int b)
{
    return (a * b);
}

enum {
    PUSH  = 1,
    ADD = 3,
    SUB = 4,
    DIV = 5,
    OUT = 6,
    HLT = 7 
};

int  main(void)
{
    //read from binary file
    int code[64] = {};
    // DO reading from file with commands and filling the code(loading the programm)
    FILE* f3 = fopen("BinExample", "rb");
    int ret = fread(code, 64, sizeof(code[0]), f3);
    fclose(f3);

    for (int i = 0; i < 64; i ++)
    {
        printf("%d ", code[i]);
    }
    return ret;
}
