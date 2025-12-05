#ifndef _WORK_
#define _WORK_

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//' ', ' ', 5

//sscanf(buff + i, "%d")
//%n

typedef struct {
    char* buff;
    size_t buff_size;
} Buffer;

typedef struct { // Структура для строки
    char* str;
    char* str_end;
} String;

size_t SizeOfFile(const char* filename);
size_t Maxlen(char* buffer);
size_t CountStr(const char* buffer);
size_t CountHash(const char* str);
int skip_space(const char* buffer);

void   OutPutBuf(char* buffer, FILE* fileout, size_t numOfElemNew);
void CreateArrPoint(String* arr_pointer, char* buffer);

Buffer CreateBuffer(const char* filename);


#endif