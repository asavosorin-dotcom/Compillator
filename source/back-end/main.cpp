#include "./back-end/main.h"

int main()
{
    Stack_t variables = {};
    INITSTACK(variables, 8);
    
    StackFunc_t functions = {};
    INITStackFunc(functions, 8);

    StackInt_t init_var = {};
    INIT_INTSTACK(init_var, 8);

    char* buffer = CreateBuffer("code_file.txt").buff;
    printf("buffer: %s\n", buffer + 1);

    CompNode_t* root = ReadNodeBack(buffer + 1, &variables, &functions);
    CompDump(root, "dump", variables);

    StackPrint(&variables);

    MakeASMCode(root, &variables, &functions, &init_var);

    DTOR(variables);
    FUNC_DTOR(functions);
    INTDTOR(init_var);
    CompDtor(root);
    free(buffer);
}