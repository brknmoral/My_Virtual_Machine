#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include <errno.h>
#include <sys/stat.h>


#define DATA_SIZE (sizeof(data)/sizeof(data[0]))

#define CODE_SIZE 28

int CodeLoad(int code[], int code_size);
int CodeDump(int code[], int code_size, int current_command);

enum {
    PUSH  = 1,
    ADD =   3,
    SUB =   4,
    DIV =   5,
    OUT =   6,
    HLT =   7 
};

int  main(void)
{
    int code[CODE_SIZE] = {};

    CodeLoad(code, CODE_SIZE);
    CodeDump(code, CODE_SIZE, 3);

    return 0;
}

int CodeLoad(int code[], int code_size)
{
    if (!code) { return -ENOMEM; }

    FILE* f = fopen ("BinExample", "rb");
    int ret = (int) fread(code, (size_t)code_size, sizeof(code[0]), f);
    fclose(f);

    return ret;
}

int CodeDump(int code[], int code_size, int current_command)
{
    for (int i = 0; i < code_size; i ++)
    {
        printf("%s%02d " RESET, (i == current_command)? RED : "", i);
    }
    printf("\n");

    for (int i = 0; i < code_size; i ++)
    {
        printf("%02d ", code[i]);
    }
    printf("\n");

    return 0;
}
