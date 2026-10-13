#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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


#define TO_VERIFY(stk1)                                                        \
{                                                                              \
    int err = Verify(stk1);                                                    \
    if (err != 0)                                                              \
    {                                                                          \
        printf("Verify Failed, err = %d, in %s:%d\n", err, __FILE__, __LINE__);\
        StackDump(stk1);                                                       \
        return err;                                                            \
    }                                                                          \
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

int StackInit     (struct Stack* stk1);
int StackPush     (struct Stack* stk1, double number);
void StackDestroy (struct Stack* stk1);
int StackPop      (struct Stack* stk1, double* number_stp);
int Verify        (struct Stack* stk1);
int StackDump     (struct Stack* stk1);
int StackOutput   (struct Stack stk1);
void BB_MemSet    (double* start, int count_elem);


int main(void)
{
    struct Stack stk1 = {};
    StackInit(&stk1);
    
    StackPush(&stk1, 10.0);
    StackPush(&stk1, 20.0);
    StackPush(&stk1, 30.0);
    StackPush(&stk1, 40.0);
    StackPush(&stk1, 50.0);
    StackPush(&stk1, 60.0);
    StackPush(&stk1, 70.0);
    StackPush(&stk1, 80.0);
    StackPush(&stk1, 90.0);

    StackOutput(stk1);
   //printf("------------------------\n");
    //StackPop(&stk1, &number_stp);
    //StackOutput(stk1);

    StackDestroy(&stk1);
    return 0;
}

int StackInit(struct Stack* stk1)
{
    if (stk1 == NULL) return STRUCT_NULL;

    (*stk1).capacity = DATA_START;

    if ((*stk1).capacity == 0)
    {
        return ZERO_CAPACITY;
    }

    (*stk1).data = (double*) calloc((size_t)(*stk1).capacity, sizeof((*stk1).data[0]));

    if ((*stk1).data == NULL)
    {
        return -ENOMEM; 
    }

    for (int i = 0; i < (*stk1).capacity; i++)
    {
        (*stk1).data[i] = poison_value;
    }
    return DATA_START;
}

void BB_MemSet(double* start, int count_elem)
{
    int i = 0;
    for(;  i < count_elem ; i++)
    {
        printf("Before : %p -> <%lg>\n", start + i, *(start + i));
        *(start + i) = poison_value;
        printf("After  : %p -> <%lg>\n", start + i, *(start + i));
    }
    return;
}

int StackPush(struct Stack* stk1, double number)
{
    TO_VERIFY(stk1);
    
    if ((*stk1).size == (*stk1).capacity)
    {
        int new_capacity = (*stk1).capacity * INCREASE;
        printf("From %d to %d\n", (*stk1).capacity , new_capacity);
        double* tmp = (double*) realloc((*stk1).data, (size_t)new_capacity * sizeof((*stk1).data[0]));
        
        if (tmp == NULL) return REALLOCAION_FAULT;
        
        (*stk1).data = tmp;
        BB_MemSet((*stk1).data + (*stk1).capacity, new_capacity - (*stk1).capacity);
        (*stk1).capacity = new_capacity;
    }
    
    //char* answer = NULL;
    //printf("FROM STACKPUSH: Count of elem: %d\n", (*stk1).size);
    //while (true){
        //printf("Do you want input number\n");
        //scanf("%s",answer);
        //if (strcmp(answer,"Yes") == 1){
            (*stk1).data[(*stk1).size] = number;
            //printf("FROM STACKPUSH: must be - %lg, get - %lg\n", number, (*stk1).data[(*stk1).size]);
            (*stk1).size++;
        //}
        //break;
    //}
    TO_VERIFY(stk1);
    return 0;
}

int StackPop(struct Stack* stk1, double* number_stp)
{
    TO_VERIFY(stk1);

    printf("Size befor pop : %d\n", (*stk1).size);
    (*stk1).size--;
    printf("Size pop elem  : %d\n", (*stk1).size);

    *number_stp = (*stk1).data[(*stk1).size];
    printf("Num_stp = %lg\n", *number_stp);

    (*stk1).data[(*stk1).size] = poison_value;

    TO_VERIFY(stk1);
    return 0;
}

void StackDestroy(struct Stack* stk1)
{
    free((*stk1).data);
    (*stk1).data =     NULL;
    (*stk1).capacity = 0;
    (*stk1).size =     0;
}

int Verify(struct Stack* stk1)
{
    if (stk1 == NULL)                    return STRUCT_NULL;
    if ((*stk1).data == NULL)            return DATA_NULL;
    if ((*stk1).size > MAX_SIZE)         return STRANGE_SIZE;

    if ((*stk1).data == NULL)            return DATA_NULL;
    if ((*stk1).size > MAX_SIZE)         return STRANGE_SIZE;
    if ((*stk1).size > (*stk1).capacity) return SIZE_OVER_CAPACITY;
    if ((*stk1).size < 0)               return ZERO_SIZE;
    
    if ((*stk1).capacity == 0)           return ZERO_CAPACITY;
    return 0;
}

int StackOutput(struct Stack stk1)
{
    printf("TO KNOW:" GREEN " *" RESET " - norm number," RED " *" RESET " - poison number\n");
    printf(GREEN "DATA OUTPUT\n" RESET);
    printf(BLUE "DED LOSHARA|    POINTER     |DATA_INDEX|   DATA_ELEM\n" RESET);
    for (int i = 0; i < stk1.capacity; i++){

        #pragma GCC diagnostic ignored "-Wfloat-equal"
        if (stk1.data[i] == poison_value){
        #pragma GCC diagnostic warning "-Wfloat-equal"

            printf(RED "PLOHOESLOVO " RESET YELLOW "<%p> -> data[%d] = %lg" RESET RED "*\n" RESET,
                 &stk1.data[i], i, stk1.data[i]);
        }
        else
            printf(RED "PLOHOESLOVO " RESET YELLOW "<%p> -> data[%d] = %lg" RESET GREEN "*\n" RESET, &stk1.data[i], i, stk1.data[i]);
    }
    return 0;
}

int StackDump(struct Stack* stk1)
{
    printf(PINK "STRUCTURE PARAMETERS\n" RESET);
    printf(GREEN "Size" RESET ":     <%p> -> %d\n", &(*stk1).size, (*stk1).size);
    printf(GREEN "Capacity" RESET": <%p> -> %d\n", &(*stk1).capacity, (*stk1).capacity);
    StackOutput(*stk1);
    return 0;
}