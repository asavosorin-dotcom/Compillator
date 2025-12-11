#include "stack_tokens.h"

size_t MaxNumOfStackTok = 4;
size_t NumOfStackTok = 0;
size_t* TokArrPointData = (size_t* ) calloc(MaxNumOfStackTok, sizeof(size_t));

int StackTokCtor(StackTok_t* stk, int capasity, int line, const char* stackTokname, const char* funcname) {
    assert(stk);
    
    #ifdef DEBUG
    stk->canary_left = KANAREYKA; 
    stk->canary_right = KANAREYKA;
    
    ArrPointData[NumOfStackTok++] = (size_t) stk->data;

    if (NumOfStackTok == MaxNumOfStackTok) {
        ReallocArrPointData();
    }

    stk->passport.line = line;
    stk->passport.filename = stackTokname;
    stk->passport.funcname = funcname;
    #endif

    stk->size = 0;
    ONDEBAGCANARY(stk->size = 1);

    #ifdef DEBUG
    if (capasity <= 0) { // capacity
        fprintf(fileerr, "Called from func: %s line: %d ERROR: capasity less than 0\n", __func__, line);
        return ERR_CAPASITY;
    }
    #endif

    stk->capasity = capasity;

    // fprintf(fileerr, "capasity = %d\n", stk->capasity);
    #ifdef CANARY
        stk->data = (StackTokElement_t* ) calloc((size_t) capasity + 2, sizeof(StackTokElement_t));
    #else
        stk->data = (StackTokElement_t* ) calloc((size_t) capasity,     sizeof(StackTokElement_t));
    #endif

    #ifdef DEBUG
    if (stk->data == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer data is NULL\n", __func__, line);
        return ERR_DATA_ADRESS;
    }
    #endif

    // STACKTokVERIFY(stk);

    // много видов условных компилляций

    ONDEBAGCANARY(stk->data[0] = KANAREYKA);

    int flag = 0;
    #ifdef DEBUG 
    ONDEBAGCANARY(flag = 1);

    for (int i = flag; i < capasity + flag; i++) { // инициализация POISON
        stk->data[i] = POISON;
    }
    #else
    for (int i = flag; i < capasity + flag; i++) { // инициализация POISON
        stk->data[i] = {};
    }
    #endif

    ONDEBAGCANARY(stk->data[capasity + 1] = KANAREYKA);

    ONDEBAGHASH(stk->hash = ((size_t) stk->capasity + stk->size + (size_t) stk->data));
    // ONDEBAGHASH(stk->hash = ((size_t) stk->capasity + stk->size));

    int err = 0;
    ERRPRINTOK("INIT OK\n")

    return 0;
}
// ver
int StackTokPush(StackTok_t* stk, StackTokElement_t elem, int line) {
    assert(stk);
    int err = 0;

    ERRPRINTOK("")

    // fprintf(fileerr, "fnvfnpwdfnvwp\n");

    // fprintf(fileerr, "size in push = %d\n", (int) stk->size);

    if ((int) stk->size == (stk->capasity - 1)) {
        // fprintf(fileerr, "Check realloc\n");
        TOKREALLOC(*stk)
    }

    // fprintf(fileerr, "Check push after realloc\n");

    // fprintf(fileerr, "\nhash befor push = %d\n", stk->hash);
    
    #ifdef HASH
    stk->hash += stk->size * (size_t) elem;
    stk->hash += 1; // size += 1
    #endif
    
    stk->data[stk->size++] = elem;

    // fprintf(fileerr, "\nhash after push = %d\n", stk->hash);

    // fprintf(fileerr, "delta hash = " TYPEELEM "\n", (StackTokElement_t) stk->size * elem);

    // fprintf(fileerr, "%d", i++);

    ERRPRINTOK("PUSH OK\n")

    return err;
}

int StackTokPop(StackTok_t* stk, StackTokElement_t* elem, int line) {
    assert(stk);
    assert(elem);

    int err = 0;
    
    // printf("before verify\n");
    ERRPRINTOK("")
    // printf("AFTER verify\n");

    #ifdef CANARY

    if (stk->size == 1) {
        // printf("Empty stackTok\n");
        *elem = 0;
        return STACKTok_EMPTY; 
    }

    #else 

    if (stk->size == 0) {
        // printf("Empty stackTok\n");
        *elem = {};
        return STACK_EMPTY; // STACKTok_EMPTY
    }

    #endif

    *elem = stk->data[--(stk->size)];

    #ifdef HASH

    stk->hash -= stk->size * (size_t) *elem;
    stk->hash -= 1; // size += 1

    #endif

    #ifdef DEBUG
        stk->data[stk->size] = POISON;
    #else 
        stk->data[stk->size] = {};
    #endif

    ERRPRINTOK("POP OK\n")

    return err;
}

int StackTokTop(StackTok_t* stk, StackTokElement_t* elem, int line) {
    
    int err = 0;
    
    ERRPRINTOK("")

   #ifdef CANARY

    if (stk->size == 1) {
        // printf("Empty stackTok\n");
        *elem = 0;
        return STACKTok_EMPTY; 
    }

    #else 

    if (stk->size == 0) {
        // printf("Empty stackTok\n");
        *elem = {};
        return STACK_EMPTY; // STACKTok_EMPTY
    }

    #endif
    
    *elem = stk->data[stk->size - 1];

    ERRPRINTOK("TOP OK\n")

    return err;
}

int StackTokDtor(StackTok_t* stk, int line) {
    
    int err = 0;

    ERRPRINTOK("")

    assert(stk);

    for (int i = 0; i < stk->capasity; i++)
    {
        free(stk->data[i]);
    }

    stk->size = 0;
    stk->capasity = 0;
    
    ONDEBAG(stk->passport = {0});

    free(stk->data);
    // printf("i WANT TO break; FREEEE!\n");
    stk->data = NULL;

    return err;

}

//0x10 
//0x55

int StackTokRealloc(StackTok_t* stk, int line) {
    int err = 0;
    
    
    ERRPRINTOK("")
    ONDEBAGHASH(stk->hash -= (size_t) stk->data);

    #ifdef CANARY
    StackTokElement_t* stkreal = (StackTokElement_t* ) realloc(stk->data, (2 * (size_t) stk->capasity + 2) * sizeof(stk->data[0]));
    #else
    StackTokElement_t* stkreal = (StackTokElement_t* ) realloc(stk->data, (2 * (size_t) stk->capasity) * sizeof(stk->data[0]));
    #endif

    // fprintf(fileerr, "stkreal[%p]\n", stkreal);

    if (stkreal != NULL) {
        stk->data = stkreal;
    }
    
    ONDEBAGHASH(stk->hash += (size_t) stk->data);

    #ifdef DEBUG
        int flag = 0;
        ONDEBAGCANARY(flag = 1);
        for (int i = stk->capasity + flag; i < stk->capasity * 2 + flag; i++) {
            // fprintf(fileerr, "i = %d\n", i);

            stk->data[i] = POISON;
        }
    #else
            for (int i = stk->capasity; i < stk->capasity * 2; i++) {
            // fprintf(fileerr, "i = %d\n", i);

            stk->data[i] = NULL;
        }
    #endif

    ONDEBAGHASH(stk->hash += (size_t) stk->capasity);

    stk->capasity = stk->capasity * 2;
    // fprintf(fileerr,"capasity = %d from realloc\n ", stk->capasity);
    
    ONDEBAGCANARY(stk->data[stk->capasity + 1] = KANAREYKA);

    ERRPRINTOK("REALLOC OK\n")

    return 0;
}

#ifdef DEBUG

void ReallocArrPointData(void) {
    size_t* newpoint = (size_t* ) realloc(ArrPointData, sizeof(size_t) * MaxNumOfStackTok);
        if (newpoint != NULL)
            ArrPointData = newpoint;
        else 
            fprintf(fileerr, "\nWrong reallocation ArrPointData!\n");
}

int StackTokDump(StackTok_t* stk, int err) {
    assert(fileerr);
    assert(stk);
    // assert(stk)

    if (!err) {
        return 0;
    }

    
    // 1 & 2;
    (err & ERR_CANARY_STRUCT) ? fprintf(fileerr, "Destroyd stackTok\n") : fprintf(fileerr, "Struct doesn't destroyd\n") ;

    ONDEBAGHASH((err & ERR_HASH) ? fprintf(fileerr, "ERROR HASH\n") : fprintf(fileerr, "HASH OK\n"));
    
    fprintf(fileerr, "stackTok[%p] %s was made in line %d function: %s{", stk, stk->passport.filename, stk->passport.line, stk->passport.funcname);
    (err & ERR_STACKTok_ADRESS) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "capasity  = %d ", stk->capasity);
    (err & ERR_CAPASITY) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "data = [%p]", stk->data);
    (err & ERR_DATA_ADRESS) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "size = %zu ", stk->size); // BAD!!!!
    (err & ERR_SIZE) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    ONDEBAGHASH((fileerr, "hash = %zu", stk->hash));


    fprintf(fileerr, "{\n");

    ONDEBAGCANARY(fprintf(fileerr, "![0] = " TYPEELEM " (KANAREYKA) ", stk->data[0]));
    (err & ERR_KANAREYKALEFT) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");
    
    int flag = 0;
    ONDEBAGCANARY(flag = 1);

    for (int i = flag; i < stk->capasity + flag; i++) {
        if (stk->data[i] == POISON)
            fprintf(fileerr, " [%d] = " TYPEELEM " (POISON)\n", i, stk->data[i]);
        else 
            fprintf(fileerr, "*[%d] = " TYPEELEM "\n", i, stk->data[i]);

    }

    ONDEBAGCANARY(fprintf(fileerr, "![%d] = " TYPEELEM " (KANAREYKA) ", stk->capasity + 1,  stk->data[stk->capasity + 1]));
    (err & ERR_KANAREYKARIGHT) ? fprintf(fileerr, "BAAAAAAAAAAAAAAAAAAAAAAAAAAD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n") : fprintf(fileerr, "\n");

    fprintf(fileerr, "}\n}\n\n");

    return err;
}

//0b00
//|
//0b01
//0b01

int StackTokVerify(StackTok_t* stk, int line, const char* funcname) {
    int err = 0;    
    // fprintf(fileerr, "verify funcname = %s \n", funcname);

    if (stk->canary_left != KANAREYKA || stk->canary_right != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: change struct \n", funcname, line);
        return ERR_CANARY_STRUCT;
    }
    
    if (stk->data == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer data is NULL\n", funcname, line);
        err = err | ERR_DATA_ADRESS;
        return ERR_DATA_ADRESS;
    }
    
    #ifdef HASH

    if (stk->hash != StackTokHash(stk)) {
        size_t hash1 = StackTokHash(stk);
        fprintf(fileerr, "Called from func: %s line: %d ERROR: change struct \n", funcname, line);
        err |= ERR_HASH;
        fprintf(fileerr, "hash = %zu\n", stk->hash);
        fprintf(fileerr, "hash1 = %zu\n", hash1);
        fprintf(fileerr, "delta hash = %zu\n", hash1 - stk->hash);
        // return ERR_HASH;
    }

    #endif

    if (stk == NULL) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: pointer stk is NULL\n", funcname, line); // called from.
        err |= ERR_STACKTok_ADRESS;
    }
    
    if (stk->capasity <= 0) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: capasity less than 0\n", funcname, line);
        err = err | ERR_CAPASITY;
    }
    // fprintf(fileerr, "size = %d", stk->size);
    if ((int) stk->size == stk->capasity + 1) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: SizeError\n", funcname, line);
        err = err | ERR_SIZE;
    }

    #ifdef CANARY

    if (stk->data[0] != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: kanareyka left is dead\n", funcname, line);
        err = err | ERR_KANAREYKALEFT;
    }

    if (stk->data[stk->capasity + 1] != KANAREYKA) {
        fprintf(fileerr, "Called from func: %s line: %d ERROR: kanareyka right is dead\n", funcname, line);
        err = err | ERR_KANAREYKARIGHT;
    }

    #endif

        // fprintf(fileerr, "funcname = %s, err = %d\n", funcname, err);
    StackTokDump(stk, err);
    
    return err;
}

#endif

#ifdef HASH

size_t StackTokHash(StackTok_t* stk) {
    size_t hash1 = 0;

    hash1 += (size_t) stk->capasity;
    hash1 += (size_t) stk->size;
    hash1 += (size_t) stk->data;

    for (size_t i = 1; i < stk->size; i++) {
        hash1 += (size_t) stk->data[i] *  i;
    } 

    for (int i = (int) stk->size; i < stk->capasity + 1; i++) {
        hash1 += (stk->data[i] == POISON) ? 0: (size_t) stk->data[i];
    }

    // printf("hash1 = %zu\n", hash1);
    return hash1;
}

#endif

// int StackTokPrint(StackTok_t* stk, int line) {
//     int err = 0;
    
//     #ifdef DEBUG
//     err = STACKTokVERIFY(stk);
//     #endif

//     #ifdef CANARY
//     for (size_t i = 1; i < stk->size; i++) {
//         printf(TYPEELEM " ", stk->data[i]);
//     }
//     #else 
//     for (size_t i = 0; i < stk->size; i++) {
//         printf(TYPEELEM " ", stk->data[i]);
//     }
//     #endif

//     printf("\n");

//     return err;
// }
