#include "./back-end/main.h"

int main()
{
    Stack_t variables = {};
    INITSTACK(variables, 8);
    
    Stack_t functions = {};
    INITSTACK(functions, 8);


    char* buffer = CreateBuffer("code_file.txt").buff;
    printf("buffer: %s\n", buffer + 1);

    CompNode_t* root = ReadNodeBack(buffer + 1, &variables, &functions);
    CompDump(root, "dump", variables);

    StackPrint(&variables);

    MakeASMCode(root, &variables);

    DTOR(variables);
    DTOR(functions);
    CompDtor(root);
    free(buffer);
}