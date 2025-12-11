#include "main.h"

int main()
{
    Stack_t variables = {};
    INITSTACK(variables, 5);
    
    StackTok_t tokens = {};
    TOKINITSTACK(tokens, 1);

    const char* s = "x = 3; x = x + 1;";
    size_t count_lex = GetLex(s, &tokens, &variables);

    printf("Token_size = %zu\n", tokens.capasity);

    CompNode_t* root = GetGeneral(&tokens);

    CompDump(root, "first dump", variables);

    DTOR(variables);
    TOKDTOR(tokens);
    fclose(file_htm);
}