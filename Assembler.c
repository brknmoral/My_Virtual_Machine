#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"

#define DATA_SIZE     (sizeof(data)/sizeof(data[0]))


#ifdef DEBUG
#define $DBG(...)     if(DbgVerbosity > 3) { __VA_ARGS__ }  // $DBUG (printf (num = %d\n", num)); --> if (DbgVerbosity > 3) printf (num = %d\n", num)
#else
#define $DBG(...)     {;}
#endif

#define COMMAND_SIZE  16
#define POISON_VALUE  -97
#define CODE_SIZE     1000

enum {
    PUSH  = 1,
    ADD = 3,
    SUB = 4,
    DIV = 5,
    OUT = 6,
    HLT = 7, 
    NO_SUCH_COMMAND = -17
};

int ReadFromFile   (const char* name, char* * text, size_t *FILE_SIZE);
int CountNumString (char* text);
void BufferOutput  (char* buffer, size_t FILE_SIZE);
int FillIndexindex (char* text, char* index[], int str_count);
int ParcingIndexMas(char** index, int end_data[], int str_count);
int CheckCommand   (char command[]);
void MasOutput     (int* data, int data_size);

int  main(void)
{
    size_t FILE_SIZE = 0;
    char* buffer = NULL;
    int ret = ReadFromFile("Assembler.txt", &buffer, &FILE_SIZE);
    BufferOutput(buffer, FILE_SIZE);

    int str_count = 0;
    str_count = CountNumString(buffer);

    char** index = (char**)calloc((size_t)str_count, sizeof(char*));// pointers
    if (index == NULL) { printf("index ptr NULL\n");return -1;}

    FillIndexindex(buffer, index, str_count);
    for (int i = 0; i < 4; i ++)
    {
        printf(">>> <%p> -> index[%d] = %p ->> {%s}\n", index + i, i, *(index + i), *(index + i));
    }

    int* result_data = (int*)calloc(CODE_SIZE, sizeof(result_data[0]));
    //int count_commands = CountNumString(buffer);
    //reading from assembler file and filling data
    int result_data_size = ParcingIndexMas(index, result_data, str_count);
    MasOutput(result_data, result_data_size);

    int data[] = {1, 30, 1, 20, 3, 1, 5, 5, 1, 4, 4, 6, 7};

    FILE* f1 = fopen("TextExamp le.txt", "wt");
    for (size_t i = 0; i < DATA_SIZE; i++)
    {
        fprintf(f1, "%d ", data[i]);
    }
    fclose(f1);
    
    FILE* f2 = fopen("BinExample", "wb");
    fwrite(data, DATA_SIZE, sizeof(data[0]), f2);
    fclose(f2);


    free(buffer);
    free(index);
    free(result_data);
    return ret;
}

int ReadFromFile(const char* name, char* * text, size_t *FILE_SIZE)
{
    printf("!\n");
    struct stat file_stat = {};
    //struct stat* tmp = &file_stat;  tmp =0x1000
    //tmp->st_size;
    // file_stat.st_size;

    int ret = 0;
    int file_descriptor = 0;
    FILE* file = NULL;
    size_t size = 0;

    if (name == NULL || text == NULL || FILE_SIZE == NULL)
    {
        ret = -EINVAL;
        goto exit;
    }

    ret = -ENOENT;
    file = fopen(name, "r");
     if (file == NULL)
     {
        fprintf(stderr, "UNpossible to open file (errno = %d)\n", errno);
        goto exit;
     }

    file_descriptor = fileno(file);
    printf("File descriptor: %d\n", file_descriptor);

    fstat(file_descriptor, &file_stat);

    *FILE_SIZE =(size_t) file_stat.st_size;
    printf("File size : %zu\n", *FILE_SIZE);

    ret = -ENOMEM;

    *text= (char *)calloc(*FILE_SIZE + 1, sizeof(char));

    if (*text == NULL)
    {
        printf("Error memory allocation\n");
        goto exit_close;
    }
    ret = -EIO;

    size = fread(*text ,sizeof(char), *FILE_SIZE, file);
    if (size != *FILE_SIZE)// Верно для строк, тк 1 байт размер char
    {
        if (feof(file))
            printf("Reached end of file (readed %zu out of %zu)\n", size, *FILE_SIZE);
        else if (ferror(file))
            perror("fread");
        goto error_free;
    }
    ret = 0;

    goto exit_close;

error_free:
    free(*text);
    *text = NULL;
exit_close:
    fclose(file);
exit:
    return ret;
}
int CountNumString(char* text)
{
    printf("!!!!!\n");
    //size_t len = strlen(text);
    size_t len   = 0;
    int count = 0;
    printf("%s\n", text);
    printf("??????\n");
    printf("%zu\n", len);
    // for (size_t i = 0; i < len; i++)
    // {
    //     printf("Count befor /n: %zu\n", count);
    //     printf("Letter : <%c>\n", text[i]);
    //     if (text[i] == '\n')
    //     {
    //         count ++;
    //         printf("Count after /n: %zu\n", count);
    //     }
    // }
    int i = 0;
    while (text[i] != '\0')
    {
        if (text[i] == '\n')
        {
            count ++;
            printf("Count after /n: %d\n", count);
        }

        i++;
        len++; 
    }

    if (len <= 0)
    {
        return -EIO;
    }

    printf("text[len = %zu] = %d <%c>\n", len - 1, text[len - 1], text[len - 1]);
    if (*(text + len) != '\n')
    {
        count += 1;
    }
    return count;
}

int FillIndexindex(char* text, char* index[], int str_count)
{ 
    int text_length = (int)strlen(text);
    printf("Str_count: %d\n", str_count);
    printf("text_length : %d\n", text_length);
    if (text == NULL || index == NULL || str_count == 0)
    {
        printf("Letter from FillIndindex:\n");
        printf("Problem ind_array, text, count of strings\n");
        return -EINVAL;
    }

#if 0
    int idx = 0;
    char *start_ptr = text;
    char *end_ptr = text + text_length;
    char *found = NULL;

    do {
        found = strchr(start_ptr, '\n');
        if (found && (found - start_ptr) > 0) {
            index[idx++] = start_ptr;
            *found = '\0';
        }
        start_ptr = found + 1;
    } while (start_ptr < end_ptr);
    
#else
    int idx = 1;
    index[0] = text;
    printf("Myau\n");
    int cur_pos = 0;
    while (cur_pos < text_length)
    {
        printf("<%d:%c> ", cur_pos, text[cur_pos]);
        if (text[cur_pos] == '\n')// text[i+1] \n?
        {
            text[cur_pos] = '\0';
            index[idx] = &text[cur_pos] + 1;//text + i + 1
            // i++; // <- Bug
            printf("I meet your slash n, idx = %d, index[%d] = <%p> -> {%s}\n", idx, idx, index[idx], index[idx]);
            idx++;
        }
        cur_pos++;
    }
    printf("Mas Index output:\n");
    for (int i = 0; i < idx; i++){
        printf("[%d] %p -> <%s>\n", i, index[i], index[i]);
    }
    printf("\n");

    // for (int i = 0, indx = 0; i < str_count; i++) {
    //     size_t tmp = strlen(index[i]);
    //     printf("i = %d; idx = %d; strlen(%s) = %zu \n", i, indx, index[i], tmp);
    //     if (tmp != 0) {
    //         index[indx++] = index[i];
    //         index[i] = NULL;
    //     }
    // }
#endif
    return idx;
}

#define EMIT_CODE_(cmd)  end_data[cur_adress++] = (cmd)

int ParcingIndexMas(char** index, int end_data[], int str_count)
{
    int current_param = POISON_VALUE;
    int cur_adress = 0;
    for (int line_num = 0; line_num < str_count; line_num++)
    {
        $DBG printf("Line number = %d\n", line_num);
        $DBG printf("$$$ index <%p> -> index[%d] = %p -> {%s} $$$\n", index + line_num, line_num, *(index + line_num), *(index + line_num));
        char command[COMMAND_SIZE] = {};
        
        int params = sscanf(index[line_num], "%15s %d", command, &current_param);
        if (params < 1) { printf("Empty srtring\n");continue; }
        printf("Command = {%s} cur_param = [%d]\n", command, current_param);

        int current_command = CheckCommand(command);
        printf("cur_com = %d\n", current_command);
        if (current_command == NO_SUCH_COMMAND) { return -EINVAL; }
        if (params >= 1)
        {
            // end_data[cur_adress++] = current_command;
            
            EMIT_CODE_ (current_command);

            if (params == 2)
            {
                EMIT_CODE_ (current_param);  // DSL(domain specific language)

                // end_data[cur_adress++] = number;
                // cur_adress = cur_adress + 2; 
            }
        }
    }
    return cur_adress;
}

#undef EMIT_CODE_

#define DO_CMD_(cmd)  if (strcasecmp(command, #cmd) == 0) { return cmd; }

int CheckCommand(char command[])
{
    DO_CMD_ (PUSH);
    DO_CMD_ (ADD);
    DO_CMD_ (SUB);
    DO_CMD_ (DIV);
    DO_CMD_ (OUT);
    DO_CMD_ (HLT);
    
    return NO_SUCH_COMMAND;
}

#undef DO_CMD_

void BufferOutput(char* buffer, size_t FILE_SIZE)
{
    for (size_t i = 0; i < FILE_SIZE; i++)
    {
        printf("<%p> -> buffer[%zu] = %c\n", &buffer[i], i, buffer[i]);
    }
}
void MasOutput(int* data, int data_size)
{
    printf("data_size = %d\n", data_size);
    for (int i = 0; i < data_size; i ++)
    {
        printf("%p -> data[%d] = %d\n", data + i, i, data[i]);
    }
}