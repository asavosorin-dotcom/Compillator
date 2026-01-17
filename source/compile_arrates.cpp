#include "compile_struct.h"

Operator_t arr_operators[OP_CAPASITY] = {
                                         "+",      CountHash("+"),       "_MATH_ADD_",    ADD   ,
                                         "-",      CountHash("-"),       "_MATH_SUB_",    SUB   ,
                                         "*",      CountHash("*"),       "_MATH_MUL_",    MUL   ,
                                         "/",      CountHash("/"),       "_MATH_DIV_",    DIV   ,
                                         "^",      CountHash("^"),       "_MATH_POW_",    DEG   ,
                                         "sin",    CountHash("sin"),     "_MATH_SIN_",    SIN   ,
                                         "cos",    CountHash("cos"),     "_MATH_COS_",    COS   ,
                                         "ln" ,    CountHash("ln") ,     "_MATH_LN_",     LN    ,
                                         "sqrt"  , CountHash("sqrt")  ,  "_MATH_SQRT_"  , SQRT  ,   
                                         "tg" ,    CountHash("tg") ,     "_MATH_TAN_",    TAN   ,
                                         "arcsin", CountHash("arcsin"),  "_MATH_ASIN_", ASIN  ,
                                         "arccos", CountHash("arccos"),  "_MATH_ACOS_", ACOS  ,
                                         "arctan", CountHash("arctan"),  "_MATH_ATAN_", ATAN  ,
                                         "sinh"  , CountHash("sinh")  ,  "_MATH_SINH_"  , SINH  , 
                                         "cosh"  , CountHash("cosh")  ,  "_MATH_COSH_"  , COSH  ,
                                         "tanh"  , CountHash("tanh")  ,  "_MATH_TANH_"  , TANH  ,
                                         "=="    , CountHash("==")    ,  "_LOG_EQUAL_"    , EQUANT,
                                         "<="    , CountHash("<=")    ,   "_LOG_LESS_EQUAL_"   , BELOW_EQ,     // Гоша лентяй
                                         ">="    , CountHash(">=")    ,   "_LOG_MORE_EQUAL_"   , ABOVE_EQ,     // Гоша лентяй
                                         "!="    , CountHash("!=")    ,   "_LOG_NOT_EQUAL_" , NO_EQUANT,     // Гоша лентяй
                                         "<"     , CountHash("<")     ,   "_LOG_LESS_", BELOW,  
                                         ">"     , CountHash(">")     ,   "_LOG_MORE_"    , ABOVE, 
                                         "="     , CountHash("=")     ,   "_ASSIGNMENT_"    , EQ   ,
                                         ";"     , CountHash(";")     ,   "_END_STATEMENT_"  , SEP  ,
                                         "if"    , CountHash("if")    ,   "_IF_"   , IF   ,
                                         "{"     , CountHash("{")     ,   "_BEGIN_OPER_"    , BEGIN,  
                                         "}"     , CountHash("}")     ,   "_END_OPER_"    , END  ,
                                         "while" , CountHash("while") ,   "_WHILE_", WHILE,
                                         "("     , CountHash("(")     ,   "_OPEN_BRACK_", PAP_OPEN,
                                         ")"     , CountHash(")")     ,   "_CLOSE_BRACK_"    , PAP_CLOSE,
                                         ","     , CountHash(",")     ,   "_COMMA_"    , COMMA, 
                                         "return", CountHash("return"),   "_RETURN_"  , RETURN,
                                         "print", CountHash("print"),   "_PRINT_"  , PRINT,
                                         "draw" , CountHash("draw"),     "_DRAW_"  , DRAW  ,
                                         "DRAWram", CountHash("DRAWram"),   "_DRAW_RAM_", DRAW_RAM, 
                                         };

// Operator_t arr_operators[OP_CAPASITY] = {
//                                          "Квазар",      CountHash("+"),       "_MATH_ADD_",    ADD   ,
//                                          "Мурмидор",      CountHash("-"),       "_MATH_SUB_",    SUB   ,
//                                          "Серендипность",      CountHash("*"),       "_MATH_MUL_",    MUL   ,
//                                          "Бздынч",      CountHash("/"),       "_MATH_DIV_",    DIV   ,
//                                          "Шпунтик",      CountHash("^"),       "_MATH_POW_",    DEG   ,
//                                          "sin",    CountHash("sin"),     "_MATH_SIN_",    SIN   ,
//                                          "cos",    CountHash("cos"),     "_MATH_COS_",    COS   ,
//                                          "ln" ,    CountHash("ln") ,     "_MATH_LN_",     LN    ,
//                                          "Жужмляк"  , CountHash("sqrt")  ,  "_MATH_SQRT_"  , SQRT  ,   
//                                          "tg" ,    CountHash("tg") ,     "_MATH_TAN_",    TAN   ,
//                                          "arcsin", CountHash("arcsin"),  "_MATH_ASIN_", ASIN  ,
//                                          "arccos", CountHash("arccos"),  "_MATH_ACOS_", ACOS  ,
//                                          "arctan", CountHash("arctan"),  "_MATH_ATAN_", ATAN  ,
//                                          "sinh"  , CountHash("sinh")  ,  "_MATH_SINH_"  , SINH  , 
//                                          "cosh"  , CountHash("cosh")  ,  "_MATH_COSH_"  , COSH  ,
//                                          "tanh"  , CountHash("tanh")  ,  "_MATH_TANH_"  , TANH  ,
//                                          "Фрикамоли"    , CountHash("==")    ,  "_LOG_EQUAL_"    , EQUANT,
//                                          "Петрикор"    , CountHash("<=")    ,   "_LOG_LESS_EQUAL_"   , BELOW_EQ,     // Гоша лентяй
//                                          "Элефантерия"    , CountHash(">=")    ,   "_LOG_MORE_EQUAL_"   , ABOVE_EQ,     // Гоша лентяй
//                                          "Облакомяу"    , CountHash("!=")    ,   "_LOG_NOT_EQUAL_" , NO_EQUANT,     // Гоша лентяй
//                                          "Шпектрель "     , CountHash("<")     ,   "_LOG_LESS_", BELOW,  
//                                          "Грохнуск"     , CountHash(">")     ,   "_LOG_MORE_"    , ABOVE, 
//                                          "Сноркель"     , CountHash("=")     ,   "_ASSIGNMENT_"    , EQ   ,
//                                          "Метанойя"     , CountHash(";")     ,   "_END_STATEMENT_"  , SEP  ,
//                                          "Колибридация"    , CountHash("if")    ,   "_IF_"   , IF   ,
//                                          "палимпсест"     , CountHash("{")     ,   "_BEGIN_OPER_"    , BEGIN,  
//                                          "бармаглот"     , CountHash("}")     ,   "_END_OPER_"    , END  ,
//                                          "Идиосинкразия" , CountHash("while") ,   "_WHILE_", WHILE,
//                                          "Нумен"     , CountHash("(")     ,   "_OPEN_BRACK_", PAP_OPEN,
//                                          "Кванфузировка"     , CountHash(")")     ,   "_CLOSE_BRACK_"    , PAP_CLOSE,
//                                          "Слэш-бздынь"     , CountHash(",")     ,   "_COMMA_"    , COMMA, 
//                                          "Глитчморфоз", CountHash("return"),   "_RETURN_"  , RETURN,
//                                          "Плюквайзер", CountHash("print"),   "_PRINT_"  , PRINT,   
//                                          };

// Variable_t arr_variable[VAR_CAPASITY] = { 
//                         "x", 0, CountHash("x"), NO,
//                         "y", 0, CountHash("y"), NO,
//                         "z", 0, CountHash("z"), NO,
// };  

// стэк с переменными, хранить ее имя, 
// стэк со структурами, c переменной ее индексом и значением, а хранить только индекс  

Type_inf_t arr_types[TYPE_CAPASITY] =  {"NUM"      , NUM     ,
                                        "OPER"       , OP     ,
                                        "VAR"      , VAR     ,
                                        "VAR_INIT" , VAR_INIT,  
                                        "FUNC"     , FUNC    ,
                                        "FUNC_INIT", FUNC_INIT,
                                    };