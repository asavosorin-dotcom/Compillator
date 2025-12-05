#include "main.h"

int main()
{
    Stack_t variables = {};
    INITSTACK(variables, 5);
    
    StackTok_t tokens = {};
    TOKINITSTACK(tokens, 1);

    const char* s = "sin(1 * 2 * x + 2)\0";
    size_t count_lex = GetLex(&s, &tokens, &variables);

    printf("Token_size = %zu\n", tokens.capasity);

    for (size_t i = 0; i < tokens.size; i++)
    {
        printf("[%zu][%p][%d]\n", i, tokens.data[i], tokens.data[i]->type);
    }

    CompNode_t* root = GetGeneral(&tokens);

    CompDump(root, "first dump", variables);

    CompDtor(root);
    DTOR(variables);
    TOKDTOR(tokens);
    fclose(file_htm);
}