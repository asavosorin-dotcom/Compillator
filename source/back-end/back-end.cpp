#include "./back-end/back-end.h"

// В RHX можно хранить последний 
// VAR_INIT  
// PUSH в таблицу имен

#define ASM_(...) fprintf(file_asm, __VA_ARGS__);

#define PUSH_(num) ASM_("PUSH %d\n", num)
#define PUSHM_(reg) ASM_("PUSHM [R%s]\n", reg) 

#define POP_(...)  ASM_("POP " __VA_ARGS__ )
#define POPR_(reg) ASM_("POPR R%s\n", reg)
#define POPM_(reg) ASM_("POPM [R%s]\n", reg)

#define _ADD_      ASM_("ADD\n")
#define _SUB_      ASM_("SUB\n")
#define _MUL_      ASM_("MUL\n")
#define _DIV_      ASM_("DIV\n")
#define _OUT_      ASM_("OUT\n")
#define _DEG_      ASM_("POW\n")
// #define 

void MakeASMCode(CompNode_t* root, Stack_t* variables)
{
    FILE* file_asm = fopen("code_asm.asm", "w");

    MakeASMNode(root, file_asm, variables);
    ASM_("HLT")
    
    fclose(file_asm);
}

void MakeASMNode(CompNode_t* node, FILE* file_asm, Stack_t* variables)
{
    if (node->left != NULL)
        MakeASMNode(node->left, file_asm, variables);

    if (node->right != NULL)
        MakeASMNode(node->right, file_asm, variables);

    switch (node->type)
    {
    case NUM:
        PUSH_(node->value.num);        
        break;
    
    case OP:
        MakeASMOP(node, file_asm);
        break;

    case VAR:
        {
            int index_var = get_index(node->value.var, variables); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            PUSH_(index_var)
            POPR_("AX") // поместили инлдекс в RAX
            PUSHM_("AX")
            break;
        }

    case VAR_INIT:
        PUSH_(node->value.index_var)
        POPR_("AX")
        PUSHM_("AX")
        break;

    default:
        break;
    }
}

#define CASE_BASE_OP(OPER)         case OPER:\
                                _##OPER##_ \
                                break;

void MakeASMOP(CompNode_t* node, FILE* file_asm)
{
    switch (node->value.oper)
    {
        CASE_BASE_OP(ADD)
        CASE_BASE_OP(SUB)
        CASE_BASE_OP(MUL)
        CASE_BASE_OP(DIV)
        CASE_BASE_OP(DEG)

        case PRINT:
            _OUT_
            break;

        case EQ:
            POPR_("HX")
            POPM_("AX")

        default:
            break;
    }
}

int get_index(char* string, Stack_t* stack)
{
    for (int index = 0; index < stack->size; index++)
    {
        if (strcmp(stack->data[index], string) == 0) return index;
    }

    return -1;
}