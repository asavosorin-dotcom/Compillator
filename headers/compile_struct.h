#ifndef _COMPILE_STRUCT_H_
#define _COMPILE_STRUCT_H_

#include "work_with_text.h"
#include "stack.h"
#include "stack_tokens.h"

#define OP_CAPASITY   21
#define TYPE_CAPASITY 4

extern struct Operator_t arr_operators[OP_CAPASITY];
extern const char* arr_types[TYPE_CAPASITY];

enum Operator_val_t {
                     ADD       = 0, 
                     SUB       = 1, 
                     MUL       = 2, 
                     DIV       = 3, 
                     DEG       = 4, 
                     SIN       = 5,  
                     COS       = 6, 
                     LN        = 7, 
                     TAN       = 8, 
                     ASIN      = 9, 
                     ACOS      = 10, 
                     ATAN      = 11, 
                     SINH      = 12,
                     COSH      = 13, 
                     TANH      = 14,

                     EQ        = 15,
                     SEP       = 16,

                     IF        = 17,
                     BEGIN     = 18,
                     END       = 19,
                     WHILE     = 20,

                     PAP_OPEN  = 50,
                     PAP_CLOSE = 51
                    };

enum Flag_t  {NO = 0, YES = 1};
enum Type_t  {NUM  = 0, OP = 1, VAR = 2, VAR_INIT = 3, FUNC = 4, FUNC_INIT = 5};

struct Variable_t
{
    char* name_var;
    int   value;
};

union Value_t
{
    Operator_val_t oper;
    int            index_var;
    int            num;
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