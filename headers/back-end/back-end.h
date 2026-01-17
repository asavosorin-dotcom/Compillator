#ifndef _BACK_END_
#define _BACK_END_

#include <stdio.h>

#include "code_tree.h"
#include "stack_int.h"

int get_index(char* string, Stack_t* stack, StackInt_t* init_var);

void MakeASMCode   (CompNode_t* root, Stack_t* variables, StackFunc_t* functions, StackInt_t* init_var);

void MakeASMNode   (CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, StackInt_t* init_var);
void MakeASMOP     (CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, StackInt_t* init_var);
int MakeAsmParams (CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions);

int MakeASMParamCallfromFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, Function_t* function);
int MakeASMParamCall(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackInt_t* init_var);
void MakeASMOPFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, int index_func);

void MakeASMBodyFunc(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions, int index_func);

int MakeAsmParams(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackFunc_t* functions);

int index_var_func (char* var, Stack_t* variables, Function_t* functions);

void MakeASM_EQ(CompNode_t* node, Stack_t* variables, FILE* file_asm, StackFunc_t* functions, StackInt_t* init_var);

void MakeASMParamFromFuncOP(CompNode_t* node, FILE* file_asm, Stack_t* variables, Function_t* functions);

void MakeASMParamDraw(CompNode_t* node, FILE* file_asm, Stack_t* variables, StackInt_t* init_var);

#endif