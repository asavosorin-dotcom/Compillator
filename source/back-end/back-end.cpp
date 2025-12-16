#include "./back-end/back-end.h"

// В RHX можно хранить последний 
// VAR_INIT  
// PUSH в таблицу имен

#define ASM_(...) fprintf(file_asm, __VA_ARGS__);

#define PUSH_(num) ASM_("PUSH %d\n", num)
#define PUSHM_(reg) ASM_("PUSHM [R%s]\n", num) 

#define POP_(...)  ASM_("POP " __VA_ARGS__ )
#define POPR_(reg) ASM_("POPR R%s\n", reg)
#define POPM_(reg) ASM_("POPM [R%s]\n")

#define _ADD_      ASM_("ADD\n")
#define _SUB_      ASM_("SUB\n")
#define _MUL_      ASM_("MUL\n")
#define _DIV_      ASM_("DIV\n")
#define _OUT_      ASM_("OUT\n")
#define _DEG_      ASM_("POW\n")
// #define 

void MakeASMCode(CompNode_t* node, FILE* file_asm, Stack_t* variables)
{
    if (node->left != NULL)
        MakeASMCode(node->left, file_asm);

    if (node->right != NULL)
        MakeASMCode(node->right, file_asm);

    switch (node->type)
    {
    case NUM:
        PUSH_("%d\n", node->value.num);        
        break;
    
    case OP:
        MakeASMOP(node, file_asm);

    case VAR:
        int index_var = ; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        PUSH_(index_var)
        POPR_("AX") // поместили инлдекс в RAX
        PUSHM_("AX")
        // PUSH index
        // POPR AX
        // PushM [AX]

    case VAR_INIT:
        PUSH()

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

        case 

        default:
            break;
    }
}