#ifndef _MIDDLE_END_
#define _MIDDLE_END_ 

#include <stdio.h>
#include <math.h>

#include "compile.h"
#include "compile_struct.h"

CompNode_t* CompOptimizConst(CompNode_t* node);
CompNode_t* CompOptimizNeytralElem(CompNode_t* node);
CompNode_t* CompOptimiz(CompNode_t* root);

int CompSolveExpresion(CompNode_t* root);

#endif