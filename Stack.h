#ifndef STACK_HEADER
#define STACK_HEADER

#define DATA_START 7 
#define INCREASE 2
#define DECREASE 
#define MAX_SIZE 100000

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define PINK    "\033[1;35m"
#define RESET   "\033[0m"


#define TO_VERIFY(stk1)     \
{                           \
    int err = Verify(stk1); \
    if (err != 0)           \
    {                       \
        StackDump(stk1);    \
        return err;         \
    }                       \
}

const double poison_value = -17;

enum ERRORS{
    STRUCT_NULL        = -13,
    DATA_NULL          = -19,
    STRANGE_SIZE       = -23,
    SIZE_OVER_CAPACITY = -29,
    ZERO_SIZE          = -31,
    ZERO_CAPACITY      = -33,
    REALLOCAION_FAULT  = -37
};


struct Stack{
    double* data;
    int size;
    int capacity;
};

int StackInit    (struct Stack* stk1, int data_capacity);
int StackPush    (struct Stack* stk1, double number);
void StackDestroy(struct Stack* stk1);
int StackPop     (struct Stack* stk1, double* number_stp);
int Verify       (struct Stack* stk1);
int StackDump    (struct Stack* stk1);
int StackOutput  (struct Stack stk1);

#endif
