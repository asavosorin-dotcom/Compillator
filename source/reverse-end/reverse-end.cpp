#include "./reverse-end/reverse-end.h"

#define CODE_(...) fprintf(file_code, __VA_ARGS__);

#define CALL_PARAMS file_code, variables, functions

void MakeCodeNode(CompNode_t* node, FILE* file_code, Stack_t* variables, StackFunc_t* functions)
{
    switch (node->type)
    {
    case NUM:
        CODE_("%d", node->value.num)
        break;
    
    case VAR:
        CODE_("%s", node->value.var)
        break;

    case VAR_INIT:
        CODE_("var %s", variables->data[node->value.index_var])
        break;

    case FUNC:
        CODE_("%s", functions->data[node->value.index_var].name)
        CODE_(" %s", arr_operators[PAP_OPEN].name);
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_("%s", arr_operators[PAP_CLOSE].name);
        break;

    case FUNC_INIT:
        CODE_("func %s", functions->data[node->value.index_var].name)
        CODE_("%s", arr_operators[PAP_OPEN].name);
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_("%s\n", arr_operators[PAP_CLOSE].name); 

        CODE_("\n%s\n", arr_operators[BEGIN].name); 
        MakeCodeNode(node->right, CALL_PARAMS);
        CODE_("\n%s\n", arr_operators[END].name); 

    case OP:
        MakeCodeOP(node, CALL_PARAMS);
        break;

    default:
        break;
    }
}

void MakeCodeOP(CompNode_t* node, FILE* file_code, Stack_t* variables, StackFunc_t* functions)
{
    switch (node->value.oper)
    {
    case ADD:
    [[fallthrogh]]
    case SUB:
    [[fallthrogh]]
    case MUL:
    [[fallthrogh]]
    case DIV:
    [[fallthrogh]]
    case DEG:
    [[fallthrogh]]
    case BELOW:
    [[fallthrogh]]
    case BELOW_EQ:
    [[fallthrogh]]
    case ABOVE:
    [[fallthrogh]]
    case ABOVE_EQ:
    [[fallthrogh]]
    case EQUANT:
    [[fallthrogh]]
    case NO_EQUANT:
    [[fallthrogh]]
    case EQ:
    [[fallthrogh]]
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_(" %s ", arr_operators[node->value.oper].name);
        MakeCodeNode(node->right, CALL_PARAMS);
        break;
    
    case COMMA:
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_("%s ", arr_operators[node->value.oper].name);
        if (node->right != NULL) MakeCodeNode(node->right, CALL_PARAMS);
        break;

    case SEP:
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_("%s\n", arr_operators[node->value.oper].name);        
        if (node->right != NULL) MakeCodeNode(node->right, CALL_PARAMS);
        break;

    case IF:
    [[fallthrough]]
    case WHILE:
        CODE_("%s ", arr_operators[node->value.oper].name);        
        CODE_("%s ", arr_operators[PAP_OPEN].name);
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_(" %s", arr_operators[PAP_CLOSE].name);

        CODE_("\n%s\n", arr_operators[BEGIN].name); 
        MakeCodeNode(node->right, CALL_PARAMS);       
        CODE_("\n%s\n\n", arr_operators[END].name);        
        break;

    case PRINT:
        CODE_("%s", arr_operators[node->value.oper].name);        
        CODE_("%s ", arr_operators[PAP_OPEN].name);
        MakeCodeNode(node->left, CALL_PARAMS);
        CODE_(" %s", arr_operators[PAP_CLOSE].name);
        break;        
        
    case RETURN:
        CODE_("%s ", arr_operators[node->value.oper].name);        
        MakeCodeNode(node->left, CALL_PARAMS);
        break;        

    default:
        break;
    }
}