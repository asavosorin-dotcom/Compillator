#ifndef _COMPIL_
#define _COMPIL_

#include <stdio.h>

#include "compile_struct.h"

CompNode_t* CompNodeCtor(Type_t type);

CompNode_t* CompNodeOPCtor(Operator_val_t oper);
CompNode_t* CompNodeNUMCtor(int val);
CompNode_t* CompNodeVARCtor(int index_var);

void CompDtor(CompNode_t* node);

#endif
