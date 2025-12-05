#include "work_with_text.h"

Buffer CreateBuffer(const char* filename) {
    
    // Buffer* point_struct = (Buffer* ) calloc(1, sizeof(Buffer));
    Buffer point_struct = {0};
    // assert(struct_buf);

    FILE* file = fopen(filename, "r");

    assert(file);

    size_t numOfElem = SizeOfFile(filename) / sizeof(char);
    char* buffer = (char* ) calloc(numOfElem + 2, sizeof(char));
    size_t numOfElemNew = fread(buffer + 1, sizeof(char), numOfElem + 1, file);

    // point_struct->buff = buffer;
    // point_struct->buff_size = numOfElemNew;

    point_struct.buff = buffer;
    point_struct.buff_size = numOfElemNew;

    fclose(file);

    return point_struct;
}

size_t SizeOfFile(const char* filename) {
    
    assert(filename);

    struct stat my_stat = {};
    int description = open(filename,  O_RDONLY);
    
    assert(description != -1);

    fstat(description, &my_stat);

    // printf("Size = %ld\n", my_stat.st_size);

    return (size_t) my_stat.st_size;
}

size_t CountStr(const char* buffer) {
    
    size_t count = 0;
    
    while (*buffer != '\0') {
        
        if (*buffer == '\n') {
            count++;
            buffer++;

            while (*buffer == '\n') {
                buffer++;
            }
        } else {
            buffer++;
        }
           
    }

    // printf("Count = %d\n\n", count + 1);

    return ++count;
}

void OutPutBuf(char* buffer, FILE* fileout, size_t numOfElemNew) {
    for (size_t i = 0; i < numOfElemNew; i++) 
        fputc(buffer[i]? buffer[i] : '\n', fileout);

}

size_t Maxlen(char* buffer) {
    
    int maxlen = 0;
    int count = 0;

    while (*buffer != '\0') {
        if (*buffer == '\n') {
            
            if (count > maxlen)
                maxlen = count;

            count = 0;
        }

        buffer++;
        count++;
    }

    printf("MaxLen = %d\n", maxlen);

    return (size_t) maxlen;
}

void CreateArrPoint(String* arr_pointer, char* buffer) {
    assert(arr_pointer);
    assert(buffer);

    char* str_char = strchr(buffer, '\n');
    // printf("%p\n", str_char);
    // printf("%ld\n", str_char - buffer);

    int i = 0; // Для того чтобы первый был null

    for (; str_char != NULL; i++){

        *str_char = '\0';
        
        (arr_pointer + i) -> str = buffer;
        (arr_pointer + i) -> str_end = str_char - 1;
        
        str_char += 1;

        while (*str_char == '\n') {
            str_char++;
        }

        buffer = str_char;
        str_char = strchr(buffer, '\n');
        // printf("Pointer [%d] %p\n", i, buffer);

    }
    
    str_char = strchr(buffer, '\0');

    arr_pointer[i].str = buffer;
    arr_pointer[i].str_end = str_char - 1;
}

// sin 
// s * 31^2 + i * 31 + n * 31

size_t CountHash(const char* str)
{
    size_t hash = 0;

    while (*str != '\0')
    {
        hash = hash * 31 + (size_t) *str;
        str++;
    }

    return hash;
}

int skip_space(const char* buffer)
{
    int pos = 0;

    while (isspace(*(buffer + pos))) {
            pos++;
        }

    return pos;
}