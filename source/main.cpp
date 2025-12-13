#include "main.h"

int main(int argc, char* argv[])
{
    Stack_t variables = {};
    INITSTACK(variables, 5);

    Stack_t functions = {};
    INITSTACK(functions, 5);

    StackTok_t tokens = {};
    TOKINITSTACK(tokens, 10);

    const char* file_name = argv[1];
    char* buffer = CreateBuffer(file_name).buff;

    // const char* s = "if (x - 3) {x = 3;} c = 5;";
    size_t count_lex = GetLex(buffer + 1, &tokens, &variables, &functions);

    for (int i = 0; i < tokens.size - 1; i++)
    {
        // PRINT_HTM("%d\n", i);
        CompDump(tokens.data[i], "token", variables);
    }

    printf("Token_size = %zu\n", tokens.capasity);

    CompNode_t* root = GetGeneral(&tokens, &variables);

    CompDump(root, "first dump", variables);

    DTOR(variables);
    DTOR(functions);
    TOKDTOR(tokens);
    fclose(file_htm);
    free(buffer);
}