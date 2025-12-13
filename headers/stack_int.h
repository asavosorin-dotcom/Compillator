#ifndef _STACK_INT_H_
#define _STACK_INT_H_

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

    #define ERRPRINTOK(str) err = STACKVERIFY(stk);     \
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

typedef int StackIntElement_t;
#define TYPEELEM "%s"

enum StackIntErr_t {STACK_INT_OK        = 0, 
                 ERR_INT_SIZE           = 1, 
                 ERR_INT_ELEM           = 2, 
                 ERR_INT_STACK_ADRESS   = 4, 
                 ERR_INT_DATA_ADRESS    = 8, 
                 ERR_INT_KANAREYKALEFT  = 16, 
                 ERR_INT_KANAREYKARIGHT = 32, 
                 ERR_INT_CAPASITY       = 64, 
                 ERR_INT_CANARY_STRUCT  = 128,
                 ERR_INT_HASH           = 256,
                 STACK_INT_EMPTY    = 512};

#ifdef DEBUG
typedef struct {
    int line;
    const char* filename;
    const char* funcname;
} Passport_t;
#endif

struct StackInt_t{
    StackIntElement_t* data;
    size_t size;
    int capasity;
    int start;    
};

int StackIntCtor(StackInt_t* stk, int capasity, int line, const char* stackname, const char* funcname);
int StackIntPush(StackInt_t* stk, StackIntElement_t elem, int line);
int StackIntPop(StackInt_t* stk, StackIntElement_t* elem, int line);
int StackIntDtor(StackInt_t* stk, int line);
// int StackIntDump(StackInt_t* stk, int err);
int StackIntTop(StackInt_t* stk, StackIntElement_t* elem, int line);
// int StackIntVerify(StackInt_t* stk, int line, const char* funcname);
int StackIntRealloc(StackInt_t* stk, int line);
// int StackIntPrint(StackInt_t* stk, int line);
void ReallocArrPointData(void);
void StackIntPrint(StackInt_t* stk);

// size_t StackHash(StackInt_t* stk);
 
#ifdef DEBUG
#define STACKVERIFY(stk) StackVerify(stk, line, __func__);
#endif

#define INIT_INTSTACK(stk, capasity) StackIntCtor(&stk, capasity, __LINE__, #stk, __func__);

#define INTPUSH(stk, elem) StackIntPush(&stk, elem, __LINE__);
#define INTPOP(stk, elem)  StackIntPop(&stk, &elem, __LINE__);
#define INTTOP(stk, elem)  StackIntTop(&stk, &elem, __LINE__);
#define INTREALLOC(stk)    StackIntRealloc(&stk, __LINE__);
#define INTDTOR(stk)       StackIntDtor(&stk, __LINE__);
#define INTPRINTSTACK(stk) StackIntPrint(&stk, __LINE__);
#define INTPRINTELEM(elem) printf(TYPEELEM"\n", elem);

// const StackElement_t POISON    = 0xDEDDEAD;
// const StackElement_t KANAREYKA = 0xDEADCA;

#endif