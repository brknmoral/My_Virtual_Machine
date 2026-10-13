#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include <errno.h>
#include <sys/stat.h>

enum {
    PUSH  = 1,
    ADD = 2,
    SUB = 3,
    DIV = 4,
    OUT = 5,
    HLT = 6 
};


int ReadFromFile(const char* name, int** text, size_t *FILE_SIZE);


int  main(void)
{
    size_t FILE_SIZE = 0;
    int* data = NULL;
    int read = ReadFromFile("test.txt", &data, &FILE_SIZE);

    return read;
}
int ReadFromFile(const char* name, int **text, size_t *FILE_SIZE)
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

    *FILE_SIZE = file_stat.st_size;
    printf("File size : %ld\n", *FILE_SIZE);

    ret = -ENOMEM;

    *text= (int *)calloc(*FILE_SIZE + 1, sizeof(int));

    if (*text == NULL)
    {
        printf("Error memory allocation\n");
        goto exit_close;
    }
    ret = -EIO;

    size = fread(*text ,sizeof(char), *FILE_SIZE, file);
    if (size * sizeof(int) != (*FILE_SIZE))
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
