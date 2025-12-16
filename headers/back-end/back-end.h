#ifndef _BACK_END_
#define _BACK_END_

#include <stdio.h>

#include "code_tree.h"

int get_index(char* string, Stack_t* stack);

void MakeASMNode(CompNode_t* node, FILE* file_asm, Stack_t* variables);
void MakeASMOP(CompNode_t* node, FILE* file_asm);

#endif