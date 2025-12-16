#ifndef _CODE_TREE_
#define _CODE_TREE_

#include <stdlib.h>

#include "compile_struct.h"
#include "stack.h"
#include "work_with_text.h"
#include "compile.h"
#include "comp_dump.h"

CompNode_t* ReadNode(char* buffer);
void CompPrintNode(const CompNode_t* node, FILE* file_print, Stack_t* variables, Stack_t* functions);
void CompPrintNodeMiddle(const CompNode_t* node, FILE* file_print);

#endif