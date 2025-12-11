#ifndef _READ_FILE_ 
#define _READ_FILE_

#include <stdio.h>
#include <string.h>


#include "compile_struct.h"
#include "compile.h"
#include "comp_dump.h"

size_t GetLex(const char* s, StackTok_t* tokens, Stack_t* variables);

CompNode_t* GetGeneral       (StackTok_t* tokens);
CompNode_t* GetEquat         (StackTok_t* tokens, int* token_pos);
CompNode_t* GetExpression    (StackTok_t* tokens, int* token_pos);
CompNode_t* GetMul           (StackTok_t* tokens, int* token_pos);
CompNode_t* GetDegree        (StackTok_t* tokens, int* token_pos);
CompNode_t* GetMathCommand   (StackTok_t* tokens, int* token_pos);
CompNode_t* GetPermissionExp (StackTok_t* tokens, int* token_pos);
CompNode_t* GetVarOrNum      (StackTok_t* tokens, int* token_pos);
CompNode_t* GetVariable      (StackTok_t* tokens, int* token_pos);
CompNode_t* GetNumber        (StackTok_t* tokens, int* token_pos);

int node_is_op(CompNode_t* node, Operator_val_t val);

#endif