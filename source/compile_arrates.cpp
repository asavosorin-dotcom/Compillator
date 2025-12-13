#include "compile_struct.h"

Operator_t arr_operators[OP_CAPASITY] = {
                                         "+",      CountHash("+"),       "ADD",    ADD  ,
                                         "-",      CountHash("-"),       "SUB",    SUB  ,
                                         "*",      CountHash("*"),       "MUL",    MUL  ,
                                         "/",      CountHash("/"),       "DIV",    DIV  ,
                                         "^",      CountHash("^"),       "DEG",    DEG  ,
                                         "sin",    CountHash("sin"),     "SIN",    SIN  ,
                                         "cos",    CountHash("cos"),     "COS",    COS  ,
                                         "ln" ,    CountHash("ln") ,     "LN",     LN   ,
                                         "tg" ,    CountHash("tg") ,     "TAN",    TAN  ,
                                         "arcsin", CountHash("arcsin"),  "ARCSIN", ASIN ,
                                         "arccos", CountHash("arccos"),  "ARCCOS", ACOS ,
                                         "arctan", CountHash("arctan"),  "ARCTAN", ATAN ,
                                         "sinh"  , CountHash("sinh")  ,  "SINH"  , SINH , 
                                         "cosh"  , CountHash("cosh")  ,  "COSH"  , COSH ,
                                         "tanh"  , CountHash("tanh")  ,  "TANH"  , TANH ,
                                         "="     , CountHash("=")     ,   "="    , EQ   ,
                                         ";"     , CountHash(";")     ,   "SEP"  , SEP  ,
                                         "if"    , CountHash("if")    ,   "IF"   , IF   ,
                                         "{"     , CountHash("{")     ,   "{"    , BEGIN,
                                         "}"     , CountHash("}")     ,   "}"    , END  ,
                                         "while" , CountHash("while") ,   "WHILE", WHILE,
                                         "("     , CountHash("(")     ,   "("    , PAP_OPEN,
                                         ")"     , CountHash(")")     ,   ")"    , PAP_CLOSE,
                                         };

// Variable_t arr_variable[VAR_CAPASITY] = { 
//                         "x", 0, CountHash("x"), NO,
//                         "y", 0, CountHash("y"), NO,
//                         "z", 0, CountHash("z"), NO,
// };  

// стэк с переменными, хранить ее имя, 
// стэк со структурами, c переменной ее индексом и значением, а хранить только индекс  

const char* arr_types[TYPE_CAPASITY] = {"NUM", "OP", "VAR", "VAR_INIT"};