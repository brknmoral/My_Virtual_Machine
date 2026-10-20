#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#include "Stack.h"

#define DATA_SIZE (sizeof(data)/sizeof(data[0]))

#define CODE_SIZE 28


struct Proc{
    int code[CODE_SIZE];
    int real_code_size;
    int pc;
    struct Stack stk1;

};

int CodeLoad(struct Proc* proc1);
int CodeDump(const struct Proc* proc1);
int ChooseOperation(struct Proc* proc1);
int RunCode(struct Proc* proc1);
int Make_Push(struct Proc* proc1);
int Make_Pop(struct Proc* proc1);
int Make_Add(struct Proc* proc1);
int Make_Sub(struct Proc* proc1);
int Make_Div(struct Proc* proc1);
int Make_Out(struct Proc* proc1);
int Make_Hlt(struct Proc* proc1);


// static int (*f[])(struct Proc* proc1) = 
// {
//     [POP]= Make_Pop, 
//     [ADD]= Make_Add ,
//     [SUB]= Make_Sub ,
//     [DIV]= Make_Div ,
//     [OUT]= Make_Out ,
//     [HLT]= Make_Hlt ,
//     [PUSH]= Make_Push,
// };

enum {
    PUSH  = 1,
    POP   = 2,
    ADD   = 3,
    SUB   = 4,
    DIV   = 5,
    OUT   = 6,
    HLT   = 7,
    NO_SUCH_COMMAND = -17
};

int main(void)
{
    struct Proc proc1 = {};

    CodeLoad(&proc1);
    StackInit(&proc1.stk1);
    CodeDump(&proc1);
    RunCode (&proc1);

    return 0;
}

int CodeLoad(struct Proc* proc1)
{
    if (!proc1) { return -EFAULT; }

    FILE* f = fopen ("BinExample", "rb");
    int ret = (int) fread((*proc1).code, sizeof((*proc1).code[0]), (size_t)CODE_SIZE, f);
    fclose(f);

    (*proc1).real_code_size = ret;
    proc1->pc = 0;

    printf("Download %s%d " RESET "elems\n", (ret !=0 ) ? PINK : "", ret);
    return ret;
}

int CodeDump(const struct Proc* proc1)
{
    for (int i = 0; i < (*proc1).real_code_size; i ++)
    {
        printf("%s%02d " RESET, (i == (*proc1).pc) ? RED : "", i);
    }
    printf("\n");

    for (int i = 0; i < (*proc1).real_code_size; i ++)
    {
        printf("%s%02d " RESET, (i == (*proc1).pc) ? RED : "" ,(*proc1).code[i]);
    }
    printf("\n");

    return 0;
}

int ChooseOperation(struct Proc* proc1)
{
    printf("-------------------------------\n");
    printf("I`m in ChooseOper\n");
    int err = 0;
    CodeDump(proc1);
    
    int current_command = proc1->code[proc1->pc++];
    printf("Cur_com = %d\n", current_command);

    switch (current_command)
    {
        case PUSH : Make_Push(proc1);  break;
        case POP  : Make_Pop (proc1);  break;
        case ADD  : Make_Add (proc1);  break;
        case SUB  : Make_Sub (proc1);  break;
        case DIV  : Make_Div (proc1);  break;
        case OUT  : Make_Out (proc1);  break;
        case HLT  : Make_Hlt (proc1);  break;
        default   : err = NO_SUCH_COMMAND; break;
    }
    return err;
}


int RunCode(struct Proc* proc1)
{
    printf("Im in Runcode\n");
    proc1->pc = 0;

    printf(RED "proc[pc](pc = %d) = %d\n" RESET, proc1->pc, proc1->code[proc1->pc]);

    do
    {
        if (ChooseOperation(proc1) != 0) { break; }
    } while (proc1->pc < proc1->real_code_size);
    return 0;
}

int Make_Push(struct Proc* proc1)
{
    printf("Im in MakePush\n");
    printf("Size = %d, Capacity = %d\n", proc1->stk1.size, proc1->stk1.capacity);
    StackPush(&proc1->stk1, proc1->code[proc1->pc++]);
    return 0;
}

int Make_Pop(struct Proc* proc1)
{
    StackPush(&proc1->stk1, proc1->code[proc1->pc++]);
    return 0;
}

int Make_Add(struct Proc* proc1)
{
    StackPush(&proc1->stk1, proc1->code[proc1->pc++]);
    return 0;
}

int Make_Sub(struct Proc* proc1)
{
    StackPush(&proc1->stk1, proc1->code[proc1->pc++]);
    return 0;
}

int Make_Div(struct Proc* proc1)
{
    StackPush(&proc1->stk1, proc1->code[proc1->pc++]);
    return 0;
}

int Make_Out(struct Proc* proc1)
{
    StackPush(&(proc1->stk1), proc1->code[proc1->pc++]);
    return 0;
}

int Make_Hlt(struct Proc* proc1)
{
    StackPush(&proc1->stk1, proc1->code[proc1->pc++]);
    return 0;
}


