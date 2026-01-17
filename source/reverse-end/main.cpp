#include "./reverse-end/main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    char* buffer = CreateBuffer(file_name).buff;
    
    Stack_t variables = {};
    INITSTACK(variables, 8);

    StackFunc_t functions = {};
    INITStackFunc(functions, 8);

    CompNode_t* root = ReadNodeBack(buffer + 1, &variables, &functions);

    FILE* file_code = fopen("reverse_code.al", "w");

    MakeCodeNode(root, file_code, &variables, &functions);

    fclose(file_code);
    CompDtor(root);
    free(buffer);
}