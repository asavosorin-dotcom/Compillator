#ifndef _StackFunc_H_
#define _StackFunc_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "compile_struct.h"
#include "Logfile.h"
#include "colors.h"

// #define DEBUG
// #define HASH
// #define CANARY

#ifdef DEBUG
    #define ONDEBAG(func) func

    #define ERRPRINTOK(str) err = StackFuncVERIFY(stk);     \
                                                        \
    if (!err)                                           \
        fprintf(fileerr, str);                          \
    else                                                \
        return err;                                     \
    

#else 
    #define ONDEBAG(func) 
    #define ERRPRINTOK(str) 
#endif

#ifdef CANARY
    #define ONDEBAGCANARY(func) func
#else
    #define ONDEBAGCANARY(func) 
#endif

#ifdef HASH
    #define ONDEBAGHASH(func) func
#else
    #define ONDEBAGHASH(func)
#endif

typedef struct Function_t StackFuncElement_t;
#define TYPEELEM "%s"

struct Function_t
{
    char* name;

    int begin;
    int end;
};

enum StackFuncErr_t {StackFunc_OK            = 0, 
                 FUNC_ERR_SIZE               = 1, 
                 FUNC_ERR_ELEM               = 2, 
                 FUNC_ERR_StackFunc_ADRESS   = 4, 
                 FUNC_ERR_DATA_ADRESS        = 8, 
                 FUNC_ERR_KANAREYKALEFT      = 16, 
                 FUNC_ERR_KANAREYKARIGHT     = 32, 
                 FUNC_ERR_CAPASITY           = 64, 
                 FUNC_ERR_CANARY_STRUCT      = 128,
                 FUNC_ERR_HASH               = 256,
                 StackFunc_EMPTY             = 512};

#ifdef DEBUG
typedef struct {
    int line;
    const char* filename;
    const char* funcname;
} Passport_t;
#endif

struct StackFunc_t{

    StackFuncElement_t* data;
    size_t size;
    int capasity; 

};

int StackFuncCtor(StackFunc_t* stk, int capasity, int line, const char* StackFuncname, const char* funcname);
int StackFuncPush(StackFunc_t* stk, StackFuncElement_t elem, int line);
int StackFuncPop(StackFunc_t* stk, StackFuncElement_t* elem, int line);
int StackFuncDtor(StackFunc_t* stk, int line);
int StackFuncDump(StackFunc_t* stk, int err);
int StackFuncTop(StackFunc_t* stk, StackFuncElement_t* elem, int line);
int StackFuncVerify(StackFunc_t* stk, int line, const char* funcname);
int StackFuncRealloc(StackFunc_t* stk, int line);
void StackFuncPrint(StackFunc_t* stk);
void ReallocArrPointData(void);


size_t StackFuncHash(StackFunc_t* stk);
 
#ifdef DEBUG
#define StackFuncVERIFY(stk) StackFuncVerify(stk, line, __func__);
#endif

#define INITStackFunc(stk, capasity) StackFuncCtor(&stk, capasity, __LINE__, #stk, __func__);

#define FUNC_PUSH(stk, elem) StackFuncPush(&stk, elem, __LINE__);
#define FUNC_POP(stk, elem)  StackFuncPop(&stk, &elem, __LINE__);
#define FUNC_TOP(stk, elem)  StackFuncTop(&stk, &elem, __LINE__);
#define FUNC_REALLOC(stk)    StackFuncRealloc(&stk, __LINE__);
#define FUNC_DTOR(stk)       StackFuncDtor(&stk, __LINE__);
#define FUNC_PRINTStackFunc(stk) StackFuncPrint(&stk, __LINE__);
#define FUNC_PRINTELEM(elem) printf(TYPEELEM"\n", elem);

// const StackFuncElement_t POISON    = 0xDEDDEAD;
// const StackFuncElement_t KANAREYKA = 0xDEADCA;

#endif