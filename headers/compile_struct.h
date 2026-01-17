#ifndef _COMPILE_STRUCT_H_
#define _COMPILE_STRUCT_H_

#include "work_with_text.h"
#include "stack.h"
#include "stack_tokens.h"

#define OP_CAPASITY   35
#define TYPE_CAPASITY 6

extern struct Operator_t arr_operators[OP_CAPASITY];
extern struct Type_inf_t arr_types[TYPE_CAPASITY];

enum Operator_val_t {
                     ADD       = 0, 
                     SUB       = 1, 
                     MUL       = 2, 
                     DIV       = 3, 
                     DEG       = 4, 
                     SIN       = 5,  
                     COS       = 6, 
                     LN        = 7,
                     SQRT      = 8,
                     TAN       = 9, 
                     ASIN      = 10, 
                     ACOS      = 11, 
                     ATAN      = 12, 
                     SINH      = 13,
                     COSH      = 14, 
                     TANH      = 15,
                     EQUANT    = 16, 
                     BELOW_EQ  = 17,
                     ABOVE_EQ  = 18,
                     NO_EQUANT = 19,
                     BELOW     = 20,
                     ABOVE     = 21,
                     EQ        = 22,
                     SEP       = 23,
                     IF        = 24,
                     BEGIN     = 25,
                     END       = 26,
                     WHILE     = 27,
                     PAP_OPEN  = 28,
                     PAP_CLOSE = 29,
                     COMMA     = 30,
                     RETURN    = 31,
                     PRINT     = 32,
                     DRAW      = 33,
                     DRAW_RAM  = 34,  
                    };

enum Flag_t  {NO = 0, YES = 1};
enum Type_t  {NUM  = 0, OP = 1, VAR = 2, VAR_INIT = 3, FUNC = 4, FUNC_INIT = 5, NO_TYPE = 6};

struct Variable_t
{
    char* name_var;
    int   value;
};

struct Type_inf_t
{
    const char* name;
    Type_t      code;
};

union Value_t
{
    Operator_val_t oper;
    int            index_var;
    int            num;
    char*          var;
};

struct Operator_t
{
    const char* name;
    size_t      hash;
    const char* command_name;
    Operator_val_t code;
};

struct CompNode_t 
{
    Type_t  type;
    Value_t value;

    CompNode_t* left;
    CompNode_t* right;
    CompNode_t* parent;
};

#define PRINT_ERR(...) printf(RED "%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n" RESET);

#endif