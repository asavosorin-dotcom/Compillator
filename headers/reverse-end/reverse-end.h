#ifndef _REVERSE_END_
#define _REVERSE_END_

#include <stdio.h>

#include "code_tree.h"

void MakeCodeNode(CompNode_t* node, FILE* file_code, Stack_t* variables, StackFunc_t* functions);
void MakeCodeOP(CompNode_t* node, FILE* file_code, Stack_t* variables, StackFunc_t* functions);

#endif