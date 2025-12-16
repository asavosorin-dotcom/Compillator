#include "./back-end/main.h"

int main()
{
    Stack_t variables = {};
    INITSTACK(variables, 8);
    
    Stack_t functions = {};
    INITSTACK(functions, 8);

    FILE* file_asm = fopen("code_asm.asm", "w");

    char* buffer = CreateBuffer("code_file.txt").buff;
    printf("buffer: %s\n", buffer + 1);

    CompNode_t* root = ReadNodeBack(buffer + 1, &variables, &functions);
    CompDump(root, "dump", variables);

    StackPrint(&variables);

    MakeASMNode(root, file_asm, &variables);

    fclose(file_asm);
    free(buffer);
}