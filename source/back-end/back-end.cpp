#include "./back-end/back-end.h"
#define PARAMS_CALL file_asm, variables, functions 

// В RHX можно хранить последний 
// VAR_INIT  
// PUSH в таблицу имен
static int count_param = 0;

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

#define _JE_       ASM_("JE ")

void MakeASMCode(CompNode_t* root, Stack_t* variables, Stack_t* functions)
{
    FILE* file_asm = fopen("code_asm.asm", "w");

    MakeASMNode(root, file_asm, variables, functions);
    ASM_("HLT")

    fclose(file_asm);
}

#define MAKE_ASM_RIGHT if (node->right != NULL) MakeASMNode(node->right, file_asm, variables, functions);
#define MAKE_ASM_LEFT if (node->left != NULL) MakeASMNode(node->left, file_asm, variables, functions);


void MakeASMNode(CompNode_t* node, FILE* file_asm, Stack_t* variables, Stack_t* functions)
{
    // if (node->left != NULL)
    //     MakeASMNode(node->left, file_asm, variables);

    //---------------------------------------------------------------------------------------------------------------
    // if (node->type == OP)
    // {
    //     if (node->value.oper == IF)
    //     {
    //         PUSH_(0)
    //         _JE_
    //         ASM_(":if\n", node);
    //     }
    // }
    //---------------------------------------------------------------------------------------------------------------


    // if (node->right != NULL)
    //     MakeASMNode(node->right, file_asm, variables);

    switch (node->type)
    {
    case NUM:
        MAKE_ASM_LEFT
        MAKE_ASM_RIGHT

        PUSH_(node->value.num);        
        break;
    
    case OP:
        MakeASMOP(node, file_asm, variables, functions);
        break;

    // переписать функцию работы прсваивания
    // 

    case VAR:
        {
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT

            int index_var = get_index(node->value.var, variables); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            PUSH_(index_var)
            POPR_("AX") // поместили инлдекс в RAX
            PUSHM_("AX")
            break;
        }

    case VAR_INIT:
        MAKE_ASM_LEFT
        MAKE_ASM_RIGHT

        PUSH_(node->value.index_var)
        POPR_("AX")
        PUSHM_("AX")
        break;

    case FUNC_INIT:
        ASM_(":%s\n", functions->data[node->value.index_var])
        MakeAsmParams(node->left, PARAMS_CALL);
    
        POPR_("EX")
        POPR_("DX")

        ASM_(";end_params\n\n\n")
        count_param = 0;
        break;


    default:
        break;
    }
}

#define CASE_BASE_OP(OPER)      case OPER:     \
                                MAKE_ASM_LEFT  \
                                MAKE_ASM_RIGHT \
                                _##OPER##_     \
                                break;

void MakeASMOP(CompNode_t* node, FILE* file_asm, Stack_t* variables, Stack_t* functions)
{
    switch (node->value.oper)
    {
        CASE_BASE_OP(ADD)
        CASE_BASE_OP(SUB)
        CASE_BASE_OP(MUL)
        CASE_BASE_OP(DIV)
        CASE_BASE_OP(DEG)

        case PRINT:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT

            _OUT_
            break;

        case EQ:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT

            POPR_("HX")
            POPM_("AX")

            break;

        case IF:
            MAKE_ASM_LEFT

            PUSH_(0)
            _JE_
            ASM_(":if%p\n", node);

            MAKE_ASM_RIGHT            

            ASM_(":if%p\n", node);
            break;

        // case COMMA:
        //     MAKE_ASM_RIGHT
        //     MAKE_ASM_LEFT

        default:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
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


// пушить в регистер возможно количество параметров, а самое главное индекс первого параметра
// можно отдельные правила для разных нод

// Я ВОТ ТУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУТ
void MakeAsmParams(CompNode_t* node, FILE* file_asm, Stack_t* variables, Stack_t* functions)
{
    if (node->right != NULL) MakeAsmParams(node->right, PARAMS_CALL); 
    
    switch (node->type)
    {
        case VAR_INIT:
            count_param++;

            PUSH_(node->value.index_var)
            PUSH_(count_param)
            break;
        
        case OP:
            if (node->value.oper == COMMA)
            {
                printf("\n\n IM IN COMMA\n");
                POPR_("EX")
                POPR_("DX")
                break;
            }
        
        default:
            count_param = 0;
            break;
    }

    if (node->left != NULL) MakeAsmParams(node->left, PARAMS_CALL);
}

// В таблице имен с функциями хрнаить индексы начала и конца области видимости 
// При вызове функции пушить в регистры индексы начала и конца 

// void MakeASMBodyFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, Stack_t* functions)
// {
//     switch (node->type)
//     {
//     case VAR:
//         // сделать функцию поиска индекса чисто в стеке
//         break;
    
//     default:
//         break;
//     }
// }

#undef PARAMS_CALL