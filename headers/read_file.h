#ifndef _READ_FILE_ 
#define _READ_FILE_

#include <stdio.h>
#include <string.h>

#include "compile_struct.h"
#include "compile.h"
#include "comp_dump.h"
#include "stack_int.h"

size_t GetLex(const char* s, StackTok_t* tokens, Stack_t* variables, Stack_t* functions);

#define PARAMS_FUNC StackTok_t* tokens, int* token_pos, StackInt_t* init_index_var, Stack_t* variables

CompNode_t* GetGeneral       (StackTok_t* tokens, Stack_t* variables);
CompNode_t* GetOperation     (PARAMS_FUNC);
CompNode_t* GetFunction      (PARAMS_FUNC);
CompNode_t* GetIf            (PARAMS_FUNC);
CompNode_t* GetEquat         (PARAMS_FUNC);
CompNode_t* GetExpression    (PARAMS_FUNC);
CompNode_t* GetMul           (PARAMS_FUNC);
CompNode_t* GetDegree        (PARAMS_FUNC);
CompNode_t* GetMathCommand   (PARAMS_FUNC);
CompNode_t* GetPermissionExp (PARAMS_FUNC);
CompNode_t* GetVarOrNum      (PARAMS_FUNC);
CompNode_t* GetVariable      (PARAMS_FUNC);
CompNode_t* GetNumber        (PARAMS_FUNC);

int node_is_op(CompNode_t* node, Operator_val_t val);

#endif