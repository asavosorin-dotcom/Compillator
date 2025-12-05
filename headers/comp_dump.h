#ifndef _COMP_DUMP_
#define _COMP_DUMP_

#include <stdio.h>
#include <stdlib.h>

#include "compile_struct.h"
#include "stack.h"
#include "stack_tokens.h"

extern FILE* file_htm;

void CompDumpNode  (CompNode_t* node, FILE* file_dump, Stack_t variables);
void CompDumpImage (CompNode_t* node, Stack_t variables);
void CompDump      (CompNode_t* node, const char* text, Stack_t variables);

#define PRINT_IMAGE(...) fprintf(file_dump, __VA_ARGS__)
#define PRINT_HTM(...)   fprintf(file_htm, __VA_ARGS__)

#endif