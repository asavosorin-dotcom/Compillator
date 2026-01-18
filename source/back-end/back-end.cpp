#include "./back-end/back-end.h"
#define PARAMS_CALL file_asm, variables, functions, init_var 

// В RHX можно хранить последний 
// VAR_INIT  
// PUSH в таблицу имен
static int count_param = 0;

#define ASM_(...) fprintf(file_asm, __VA_ARGS__);

#define PUSH_(num) ASM_("PUSH %d\n", num)
#define PUSHR_(reg) ASM_("PUSHR %s\n", reg)
#define PUSHM_(reg) ASM_("PUSHM [R%s]\n", reg) 

// #define POP_(...)  ASM_("POP " __VA_ARGS__ )
#define POPR_(reg) ASM_("POPR R%s\n", reg)
#define POPM_(reg) ASM_("POPM [R%s]\n", reg)

#define CALL_(label) ASM_("CALL :%s\n", label)

#define _ADD_           ASM_("ADD\n")
#define _SUB_           ASM_("SUB\n")
#define _MUL_           ASM_("MUL\n")
#define _DIV_           ASM_("DIV\n")
#define _OUT_           ASM_("OUT\n")
#define _DEG_           ASM_("POW\n")

#define _SQRT_          ASM_("SQRT\n")

#define _JE_            ASM_("JE ")
#define _JNE_            ASM_("JNE ")

#define _J_             ASM_("J ")
#define _JBE_ ASM_("JBE ")
#define _JB_ ASM_("JB ")
#define _JA_ ASM_("JA ")
#define _JAE_ ASM_("JAE ")

#define _RET_      ASM_("RET\n")

void MakeASMCode(CompNode_t* root, Stack_t* variables, StackFunc_t* functions, StackInt_t* init_var)
{
    FILE* file_asm = fopen("code_asm.asm", "w");

    MakeASMNode(root, file_asm, variables, functions, init_var);
    ASM_("HLT")

    fclose(file_asm);
}

#define MAKE_ASM_RIGHT if (node->right != NULL) MakeASMNode(node->right, file_asm, variables, functions, init_var);
#define MAKE_ASM_LEFT if (node->left != NULL) MakeASMNode(node->left, file_asm, variables, functions, init_var);


void MakeASMNode(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, StackInt_t* init_var)
{

    switch (node->type)
    { 
    case NUM:
        MAKE_ASM_LEFT
        MAKE_ASM_RIGHT

        PUSH_(node->value.num);        
        break;
    
    case OP:
        MakeASMOP(node, file_asm, variables, functions, init_var);
        break;

    // переписать функцию работы прсваивания
    // 

    case VAR:
        {
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT

            int index_var = get_index(node->value.var, variables, init_var); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            PUSH_(index_var)
            POPR_("AX") // поместили инлдекс в RAX
            PUSHM_("AX")
            break;
        }

    case VAR_INIT:
        MAKE_ASM_LEFT
        MAKE_ASM_RIGHT

        INTPUSH(*init_var, node->value.index_var)

        PUSH_(node->value.index_var)
        POPR_("AX")
        PUSHM_("AX")
        break;

    case FUNC_INIT: // вот тут надо добавлять стековый фрейм 
    {
        _J_
        ASM_(":end%s\n", functions->data[node->value.index_var].name)
        ASM_(":%s\n", functions->data[node->value.index_var].name)
        int index_begin = MakeAsmParams(node->left, file_asm, variables, functions);
    
        functions->data[node->value.index_var].begin = index_begin;
        functions->data[node->value.index_var].end   = index_begin + count_param;

        printf("function: name [%s] begin [%d] end [%d]\n", functions->data[node->value.index_var].name, index_begin, index_begin + count_param);

        POPR_("EX")
        POPR_("DX")

        ASM_(";end_params\n\n\n")
        count_param = 0;

        MakeASMBodyFunc(node->right, file_asm, variables, functions, node->value.index_var);       

        ASM_(":end%s\n", functions->data[node->value.index_var].name)

        break;
    }

    case FUNC:
    {
        int count_param = MakeASMParamCall(node->left, file_asm, variables, init_var);
        int index_ram = functions->data[node->value.index_var].begin;

        for (int count = 0; count < count_param; count++)
        {
            PUSH_(index_ram + count)
            POPR_("BX")
            POPM_("BX")
        }

        CALL_(functions->data[node->value.index_var].name)
    }

    default:
        break;
    }
}

#define CASE_BASE_OP(OPER)      case OPER:     \
                                MAKE_ASM_LEFT  \
                                MAKE_ASM_RIGHT \
                                _##OPER##_     \
                                break;

void MakeASMOP(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, StackInt_t* init_var)
{
    switch (node->value.oper)
    {
        CASE_BASE_OP(ADD)
        CASE_BASE_OP(SUB)
        CASE_BASE_OP(MUL)
        CASE_BASE_OP(DIV)
        CASE_BASE_OP(DEG)
        CASE_BASE_OP(SQRT)

        case PRINT:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT

            _OUT_
            break;

        case DRAW:
            MakeASMParamDraw(node->left, file_asm, variables, init_var);
            POPR_("CX")
            POPM_("CX")

            break;
    
        case DRAW_RAM:
            ASM_("DRAW\n")
            break;

        case EQ:
            MakeASM_EQ(node, variables, file_asm, functions, init_var);
            break;

        case IF:
            MAKE_ASM_LEFT

            ASM_(":if%p\n", node);

            MAKE_ASM_RIGHT            

            ASM_(":if%p\n", node);
            break;

        case BELOW:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
            _JAE_ 
            break;

        case BELOW_EQ:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
            _JA_ 
            break;

        case ABOVE:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
            _JBE_ 
            break;

        case ABOVE_EQ:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
            _JB_ 
            break;

        case EQUANT:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
            _JNE_ 
            break;

        case NO_EQUANT:
            MAKE_ASM_LEFT
            MAKE_ASM_RIGHT
            _JE_ 
            break;

        case WHILE:
            ASM_(":whstart%p\n", node)
            MAKE_ASM_LEFT

            ASM_(":wh%p\n", node);

            MAKE_ASM_RIGHT            

            _J_
            ASM_(":whstart%p\n", node);
            ASM_(":wh%p\n", node);
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

int get_index(char* string, Stack_t* stack, StackInt_t* init_var) 
{
    for (int index = 0; index < init_var->size; index++)
    {
        if (strcmp(stack->data[init_var->data[index]], string) == 0) return init_var->data[index];
    }

    return -1;
}

// пушить в регистер возможно количество параметров, а самое главное индекс первого параметра
// можно отдельные правила для разных нод

// Я ВОТ ТУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУУТ
int MakeAsmParams(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions)
{
    int first_index = 0;
    
    if (node->right != NULL) first_index = MakeAsmParams(node->right, file_asm, variables, functions); 
    
    switch (node->type)
    {
        case VAR_INIT:
            count_param++;

            PUSH_(node->value.index_var)
            PUSH_(count_param)
            first_index = node->value.index_var;
            break;
        
        case OP:
            if (node->value.oper == COMMA)
            {
                // printf("\n\n IM IN COMMA\n");
                POPR_("EX")
                POPR_("DX")
                break;
            }
        
        default:
            // count_param = 0;
            break;
    }

    if (node->left != NULL) first_index = MakeAsmParams(node->left, file_asm, variables, functions);

    return first_index;
}

void MakeASMParamDraw(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackInt_t* init_var)
{
    if (node->right != NULL) MakeASMParamDraw(node->right, file_asm, variables, init_var);
    if (node->left != NULL) MakeASMParamDraw(node->left, file_asm, variables, init_var);
    
    switch (node->type)
    {
    case NUM:
        PUSH_(node->value.num)
        break;
    
    case VAR:
    {
        int index = get_index(node->value.var, variables, init_var);
        PUSH_(index)
        POPR_("AX")
        PUSHM_("AX")
        break;
    }

    default:
        break;
    }
}

// В таблице имен с функциями хрнаить индексы начала и конца области видимости 
// При вызове функции пушить в регистры индексы начала и конца 

void MakeASMBodyFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, int index_func)
{
    switch (node->type)
    {
    case NUM:
        PUSH_(node->value.num)

        break;
    
    case VAR:
    {
        int index_var = index_var_func(node->value.var, variables, &functions->data[index_func]);
        PUSH_(index_var)
        
        PUSHM_("HX")
        _ADD_

        POPR_("AX") // поместили индекс в RAX
        PUSHM_("AX")

        break;
    }

    case VAR_INIT:
    {
        functions->data[index_func].end++; // так как все переменные инициализируются по порядку (если нет, то пиздец)
        PUSH_(node->value.index_var)

        PUSHM_("HX")
        _ADD_

        POPR_("AX") 
        PUSHM_("AX")

        break;
    }
    case OP:
        MakeASMOPFunc(node, file_asm, variables, functions, index_func);
        break;

    case FUNC:
    {
        PUSHR_("HX")
        PUSH_(functions->data[index_func].end - functions->data[index_func].begin)
        _ADD_

        POPR_("HX")
        
        int index_ram = functions->data[node->value.index_var].begin;
        int count_param = MakeASMParamCallfromFunc(node->left, file_asm, variables, &functions->data[index_func]);

        for (int count = 0; count < count_param; count++)
        {
            PUSH_(index_ram + count)
            POPR_("BX")
            POPM_("BX")
        }

        CALL_(functions->data[node->value.index_var].name)
        break;
    }

    default:
        break;
    }
}

#define MAKE_ASM_LEFT_FUNC  if (node->left != NULL) MakeASMBodyFunc(node->left, file_asm, variables, functions, index_func);
#define MAKE_ASM_RIGHT_FUNC if (node->right != NULL) MakeASMBodyFunc(node->right, file_asm, variables, functions, index_func);

int MakeASMParamCallfromFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, Function_t* function)
{
    int count_param = 0;
    
    switch (node->type)
    {
        case VAR:
        {            
            int index_var = index_var_func(node->value.var, variables, function);
            PUSH_(index_var)
            PUSHM_("HX")
            _ADD_

            POPR_("AX")
            PUSHM_("AX")

            count_param++;
            break;
        }

        case NUM:
            PUSH_(node->value.num)
            count_param++;
            break;

        case OP:
            MakeASMParamFromFuncOP(node, file_asm, variables, function);
            break;


        default:
            break;
    }

    return count_param;
}

int MakeASMParamCall(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackInt_t* init_var)
{
    int count_param = 0;

    if (node->right != NULL) MakeASMParamCall(node->right, file_asm, variables, init_var);
    if (node->left  != NULL) MakeASMParamCall(node->left,  file_asm, variables, init_var);
    
    switch (node->type)
    {
        case VAR:
        {
            int index_var = get_index(node->value.var, variables, init_var);
            PUSH_(index_var)
            POPR_("AX")
            PUSHM_("AX")

            count_param++;
            break;
        }

        case NUM:
            PUSH_(node->value.num)
            count_param++;
            break;

        default:
            break;
    }

    return count_param;
}

#define CASE_BASE_OP(OPER)      case OPER:          \
                                MAKE_ASM_LEFT_FUNC  \
                                MAKE_ASM_RIGHT_FUNC \
                                _##OPER##_          \
                                break;

void MakeASMOPFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, int index_func)
{
    switch (node->value.oper)
    {
        CASE_BASE_OP(ADD)
        CASE_BASE_OP(SUB)
        CASE_BASE_OP(MUL)
        CASE_BASE_OP(DIV)
        CASE_BASE_OP(DEG)
        CASE_BASE_OP(SQRT)

        case PRINT:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC

            _OUT_
            break;

        case DRAW:
            ASM_(";DRAWWWWWWWWWW\n\n")
            printf("draaaaaaaaaaaaaaaaaaaaaaw\n\n");
            MakeASMParamCallfromFunc(node->left, file_asm, variables, &functions->data[index_func]);
            POPR_("CX")
            POPM_("CX")
            ASM_(";END_DRAW\n\n")
            break;

        case EQ:
            MAKE_ASM_RIGHT_FUNC
            MAKE_ASM_LEFT_FUNC

            POPR_("HX")
            POPM_("AX")

            break;

        case IF:
            MAKE_ASM_LEFT_FUNC

            ASM_(":if%p\n", node);

            MAKE_ASM_RIGHT_FUNC        

            ASM_(":if%p\n", node);
            break;

        case BELOW:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            _JAE_ 
            break;

        case BELOW_EQ:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            _JA_ 
            break;

        case ABOVE:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            _JBE_ 
            break;

        case ABOVE_EQ:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            _JB_ 
            break;

        case EQUANT:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            _JNE_ 
            break;

        case NO_EQUANT:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            _JE_ 
            break;

        case RETURN:
            MAKE_ASM_LEFT_FUNC

            PUSHR_("HX")
            PUSH_(functions->data[index_func].end - functions->data[index_func].begin)
            _SUB_
            POPR_("HX")

            _RET_
            break;
        // case COMMA:
        //     MAKE_ASM_RIGHT
        //     MAKE_ASM_LEFT

        case WHILE:
            ASM_(":whstart%p\n", node)
            MAKE_ASM_LEFT_FUNC

            PUSH_(0)
            _JNE_
            ASM_(":wh%p\n", node);

            MAKE_ASM_RIGHT_FUNC            

            _J_
            ASM_(":whstart%p\n", node);
            ASM_(":wh%p\n", node);
            break;

        default:
            MAKE_ASM_LEFT_FUNC
            MAKE_ASM_RIGHT_FUNC
            break;
    }
}

void MakeASM_EQ(CompNode_t* node, Stack_t* variables, FILE* file_asm, StackFunc_t* functions, StackInt_t* init_var)
{
    
    ASM_(";присваивание\n");
    
    MAKE_ASM_RIGHT
    MAKE_ASM_LEFT

    POPR_("HX")
    POPM_("AX")

}

// void MakeASMDrawParam()

int index_var_func (char* var, Stack_t* variables, Function_t* functions)
{
    for (int index = functions->begin; index < functions->end; index++)
    {
        if (strcmp(var, variables->data[index]) == 0)
            return index;
    }

    PRINT_ERR("Variable [%s] doesn't exist\n", var);
    return -1;
}

void MakeASMParamFromFuncOP(CompNode_t* node, FILE* file_asm, Stack_t* variables, Function_t* functions)
{
    switch (node->value.oper)
    {
        case ADD:
        if (node->right != NULL) MakeASMParamCallfromFunc(node->right, file_asm, variables, functions);
        if (node->left  != NULL) MakeASMParamCallfromFunc(node->left,  file_asm, variables, functions);
            _ADD_
            break;

        case SUB:
        if (node->left  != NULL) MakeASMParamCallfromFunc(node->left,  file_asm, variables, functions);
        if (node->right != NULL) MakeASMParamCallfromFunc(node->right, file_asm, variables, functions);
            _SUB_
            break;

        case MUL:
            if (node->left  != NULL) MakeASMParamCallfromFunc(node->left,  file_asm, variables, functions);
            if (node->right != NULL) MakeASMParamCallfromFunc(node->right, file_asm, variables, functions);
            _MUL_
            break;

        case DIV:
            if (node->left  != NULL) MakeASMParamCallfromFunc(node->left,  file_asm, variables, functions);
            if (node->right != NULL) MakeASMParamCallfromFunc(node->right, file_asm, variables, functions);
            _DIV_
            break;

        case DEG:
            if (node->left  != NULL) MakeASMParamCallfromFunc(node->left,  file_asm, variables, functions);
            if (node->right != NULL) MakeASMParamCallfromFunc(node->right, file_asm, variables, functions);
            _DEG_
            break;

    }
}

#undef PARAMS_CALL