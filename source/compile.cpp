#include "compile.h"

CompNode_t* CompNodeCtor(Type_t type)
{
    CompNode_t* node = (CompNode_t* ) calloc(1, sizeof(CompNode_t));

    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->value = {};

    return node;
}

CompNode_t* CompNodeEmptyCtor(void)
{
    CompNode_t* node = (CompNode_t* ) calloc(1, sizeof(CompNode_t));

    return node;
}

CompNode_t* CompNodeOPCtor(Operator_val_t oper)
{
    CompNode_t* node = CompNodeCtor(OP);

    node->value.oper = oper;

    return node;
}

CompNode_t* CompNodeNUMCtor(int val)
{
    CompNode_t* node = CompNodeCtor(NUM);

    node->value.num = val;

    return node;
}

CompNode_t* CompNodeVARCtor(char* var)
{
    CompNode_t* node = CompNodeCtor(VAR);

    node->value.var = var;

    return node;
}

void CompDtor(CompNode_t* node)
{    
    static int counter = 0;
    
    printf("node_free[%p]\n", node);
    
    if ((node->type == VAR) || (node->type == FUNC))
        free(node->value.var);
        
    if (node->left != NULL)
        CompDtor(node->left);

    if (node->right != NULL)
        CompDtor(node->right);
    
    // printf("[%d]\n", counter);
    free(node);
}