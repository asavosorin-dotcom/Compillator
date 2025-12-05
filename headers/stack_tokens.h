#ifndef _STACK_TOK_H_
#define _STACK_TOK_H_

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

typedef struct CompNode_t* StackTokElement_t;

enum StackTokErr_t {TOK_STACK_OK           = 0, 
                    TOK_ERR_SIZE           = 1, 
                    TOK_ERR_ELEM           = 2, 
                    TOK_ERR_STACK_ADRESS   = 4, 
                    TOK_ERR_DATA_ADRESS    = 8, 
                    TOK_ERR_KANAREYKALEFT  = 16, 
                    TOK_ERR_KANAREYKARIGHT = 32, 
                    TOK_ERR_CAPASITY       = 64, 
                    TOK_ERR_CANARY_STRUCT  = 128,
                    TOK_ERR_HASH           = 256,
                    TOK_STACK_EMPTY        = 512};

#ifdef DEBUG
typedef struct {
    int line;
    const char* filename;
    const char* funcname;
} Passport_t;
#endif

struct StackTok_t
{
    ONDEBAG(StackTokElement_t canary_left); // условная компилляция
    ONDEBAG(size_t NumOfStack);
    ONDEBAGHASH(size_t hash);

    StackTokElement_t* data;
    size_t size;
    int capasity; 

    ONDEBAG(Passport_t passport);
    ONDEBAG(StackTokElement_t canary_right);
    
};

int StackTokCtor    (StackTok_t* stk, int capasity, int line, const char* stackname, const char* funcname);
int StackTokPush    (StackTok_t* stk, StackTokElement_t elem, int line);
int StackTokPop     (StackTok_t* stk, StackTokElement_t* elem, int line);
int StackTokDtor    (StackTok_t* stk, int line);
int StackTokDump    (StackTok_t* stk, int err);
int StackTokTop     (StackTok_t* stk, StackTokElement_t* elem, int line);
int StackTokVerify  (StackTok_t* stk, int line, const char* funcname);
int StackTokRealloc (StackTok_t* stk, int line);
int StackTokPrint   (StackTok_t* stk, int line);
void TokReallocArrPointData(void);


size_t StackHash(StackTok_t* stk);
 
#ifdef DEBUG
#define STACKVERIFY(stk) StackVerify(stk, line, __func__);
#endif

#define TOKINITSTACK(stk, capasity) StackTokCtor(&stk, capasity, __LINE__, #stk, __func__);

#define TOKPUSH(stk, elem) StackTokPush(&stk, elem, __LINE__);
#define TOKPOP(stk, elem)  StackTokPop(&stk, &elem, __LINE__);
#define TOKTOP(stk, elem)  StackTokTop(&stk, &elem, __LINE__);
#define TOKREALLOC(stk)    StackTokRealloc(&stk, __LINE__);
#define TOKDTOR(stk)       StackTokDtor(&stk, __LINE__);
#define TOKPRINTSTACK(stk) StackTokPrint(&stk, __LINE__);

// const StackTokElement_t POISON    = 0xDEDDEAD;
// const StackTokElement_t KANAREYKA = 0xDEADCA;

#endif