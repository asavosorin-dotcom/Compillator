#include "main.h"

int main(int argc, char* argv[])
{
    Stack_t variables = {};
    INITSTACK(variables, 5);
    
    StackTok_t tokens = {};
    TOKINITSTACK(tokens, 1);

    const char* file_name = argv[1];
    char* buffer = CreateBuffer(file_name).buff;

    // const char* s = "if (x - 3) {x = 3;} c = 5;";
    size_t count_lex = GetLex(buffer + 1, &tokens, &variables);

    printf("Token_size = %zu\n", tokens.capasity);

    CompNode_t* root = GetGeneral(&tokens);

    CompDump(root, "first dump");

    DTOR(variables);
    TOKDTOR(tokens);
    fclose(file_htm);
    free(buffer);
}