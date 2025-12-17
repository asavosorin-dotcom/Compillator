#include "read_file.h"

int ifTokenMath(CompNode_t* token);
int isvariable(char* str, Stack_t* variables);
int isfunction(char* name, Stack_t functions);
int get_len_name(const char* s);
int var_init(char* var, StackInt_t* init_index_var, Stack_t variables);
// положение токена
// логичсеская часть
// добавить функции 
// добавить запятые
// добавить return
// написать запись дерева
// оформить в виде папки front-end

// function () {dsaslkvn; a; lkdvn; DGF;}

// хранить название переменной, при инициализации пушить в стек
// не забыть про хранение названий функций и переменных (ПОКА БЕЗ ЭТОГО!!!!!!!)

#define aa printf("%s:%d [%d]push node = [%p]\n", __FILE__, __LINE__, tokens->size, node);
#define $ printf("%s:%d [%d]token = [%p]\n", __FILE__, __LINE__, *token_pos, Token); 

#define PARAMS_FUNC_CALL tokens, token_pos, init_index_var, variables

#define Token tokens->data[*token_pos]
#define TOKEN_NULL if (Token == NULL) return NULL;

size_t GetLex(const char* s, StackTok_t* tokens, Stack_t* variables, Stack_t* functions)
{
    size_t count_lex = 0;
    
    CompNode_t* node = NULL;

    
    while (*s != '\0')
    {
        printf("[%s]\n", s);

        if (*s == '(')
        {
            node = CompNodeOPCtor(PAP_OPEN);

            TOKPUSH(*tokens, node);

             // OP value = PAP_OPEN
            s++;
            continue;
        }

        if (*s == ')')
        {
            node = CompNodeOPCtor(PAP_CLOSE);
            TOKPUSH(*tokens, node);
            aa

            s++;
            continue;
        }

        if (strncmp(s, "if", 2) == 0)
        {
            node = CompNodeOPCtor(IF);
            TOKPUSH(*tokens, node);

            s += 2;
            continue;
        }

        if (strncmp(s, "while", 5) == 0)
        {
            node = CompNodeOPCtor(WHILE);
            TOKPUSH(*tokens, node);

            s += 5;
            continue;
        }

        if (strncmp(s, "print", 5) == 0)
        {
            node = CompNodeOPCtor(PRINT);
            TOKPUSH(*tokens, node);

            s += 5;
            continue;
        }

        if (strncmp(s, "var", 3) == 0) // переделать
        {
            node = CompNodeCtor(VAR_INIT);
            s += 3;
            s += skip_space(s);

            if (!isalpha(*s))
            {
                PRINT_ERR("Syntax error\n");
                return 0;
            }

            int var_len = get_len_name(s);

            char* name_var = strndup(s, var_len);

            s += var_len;
            PUSH(*variables, name_var);
 
            node->value.index_var = variables->size - 1;
            aa
            TOKPUSH(*tokens, node);

            continue;
        }

        if (strncmp(s, "func", 4) == 0)
        {
            node = CompNodeCtor(FUNC_INIT);
            
            s += 4;
            s += skip_space(s);
            
            int len_func_name = get_len_name(s); 
            char* func_name  = strndup(s, len_func_name);
            s += len_func_name;
            
            PUSH(*functions, func_name);
            node->value.index_var = functions->size - 1;
            
            TOKPUSH(*tokens, node);
            continue;
        }

        if (*s == '{')
        {
            node = CompNodeOPCtor(BEGIN);
            TOKPUSH(*tokens, node);

            s++;
            continue;
        }

        if (*s == '}')
        {
            node = CompNodeOPCtor(END);
            TOKPUSH(*tokens, node);

            node = CompNodeOPCtor(SEP);
            TOKPUSH(*tokens, node);

            s++;
            continue;
        }

        if (*s == '=')
        {
            node = CompNodeOPCtor(EQ);
            TOKPUSH(*tokens, node);

            s++;
            continue;
        }

        if (*s == ';')
        {
            node = CompNodeOPCtor(SEP);
            TOKPUSH(*tokens, node);
            aa

            s++;
            continue;
        }

        if (*s == ',')
        {
            node = CompNodeOPCtor(COMMA);
            TOKPUSH(*tokens, node);

            s++;
            continue;
        }

        if (strncmp(s, "return", 6) == 0)
        {
            node = CompNodeOPCtor(RETURN);
            TOKPUSH(*tokens, node);

            s += 6;
            continue;
        }

        if (isdigit(*s)) // пока только int
        {
            int value = 0;
            do {
                value = 10 * value + (*s - '0');
                s++;
            } while (isdigit(*s));

            node = CompNodeNUMCtor(value);
            TOKPUSH(*tokens, node);

            continue;
        }

        int command_flag = NO;
        for (int op_index = 0; op_index < OP_CAPASITY; op_index++) // можно в массив оперетаров добавить длину оператора
        {

            // printf("[%d]s = [%s]\n", op_index, *s);
            if (strncmp(s, arr_operators[op_index].name, strlen(arr_operators[op_index].name)) == 0) // не может быть переменной с названием как стандартная функция
            {

                node = CompNodeOPCtor(arr_operators[op_index].code);
                TOKPUSH(*tokens, node);
                aa

                command_flag = YES;
                // printf("я тут есть\n");
                s += strlen(arr_operators[op_index].name);
                break;
            }
        }

        if (command_flag) continue;

        if (isalpha(*s))
        {    
            int var_len = get_len_name(s);

            // тут будет условие для функции
            
            char* name = strndup(s, var_len);
            s += var_len;

            int index_func = isfunction(name, *functions);

            if (index_func != -1)
            {
                node = CompNodeCtor(FUNC);
                node->value.index_var = index_func;
                TOKPUSH(*tokens, node);
                free(name);
                continue;
            }

            int index_var = isvariable(name, variables);
            
            if (index_var == -1)
            {
                PRINT_ERR("Uninitialized variable [%s]", name);
                continue;
            }

            node = CompNodeVARCtor(name);
            TOKPUSH(*tokens, node);
            aa
            
            continue;
        }

        if (isspace(*s)) {s++; continue;}

        PRINT_ERR("Syntax_error: %c\n", *s);
    }

    // printf("---------------------------------------\n");
    StackPrint(functions);

    printf(BOLD_GREEN "Tokens SUCCESS!\n" RESET);
    return tokens->size;
}

int isvariable(char* str, Stack_t* variables)
{
    for (int index_var = 0; index_var < variables->size; index_var++)
    {
        if (strcmp(str, variables->data[index_var]) == 0)
            return index_var;
    }

    return -1;
}

int get_len_name(const char* s)
{
    int var_len = 0;

    do {
        var_len++;
        s++;
    } while (isalnum(*s));

    return var_len;
}

CompNode_t* GetGeneral(StackTok_t* tokens, Stack_t* variables)
{
    StackInt_t init_index_var = {};
    INIT_INTSTACK(init_index_var, 7);
    
    int token_pos = 0;
    CompNode_t* node = GetOperation(tokens, &token_pos, &init_index_var, variables);

    INTDTOR(init_index_var);
    if (tokens->data[token_pos] == NULL)
    {
        printf(BOLD_GREEN "Parsing SUCCESS!\n" RESET);
        return node;
    }
    
    PRINT_ERR("Syntax error");
    return NULL;
}

// Sum   -> Mul +|- Mul
// IF    -> "if" (Expression) '{' {IF|EQ}+ '}'
// Equat -> {{VAR '='} Expression} ';' | Expression 

CompNode_t* GetOperation(PARAMS_FUNC)
{
    TOKEN_NULL
    
    CompNode_t* node_left = GetIf(PARAMS_FUNC_CALL);
    if (node_left == NULL) node_left = GetFunction(PARAMS_FUNC_CALL);
    if (node_left == NULL) node_left = GetReturn  (PARAMS_FUNC_CALL);
    if (node_left == NULL) node_left = GetPrint   (PARAMS_FUNC_CALL);
    if (node_left == NULL) node_left = GetEquat   (PARAMS_FUNC_CALL);

    if (node_left == NULL)
    {
        // (*token_pos)++;
        return NULL;
    }

    // CompDump(Token, "token sep");
    CompNode_t* sep = Token;
    
    (*token_pos)++;
    
    if (!node_is_op(sep, SEP)) 
    {
        PRINT_ERR("Syntax error SEP");
        return NULL;
    }
    
    sep->left = node_left;
    
    CompNode_t* node_right = GetOperation(PARAMS_FUNC_CALL);
    sep->right = node_right;
    
    // CompDump(sep, "sep");
    return sep;
}

#define check_for(enum, lex) if (!node_is_op(Token, enum))                         \
                             {                                                     \
                                 PRINT_ERR("Syntax error in \"" lex "\"\n");       \
                                 return NULL;                                      \
                             }                                                     \
                             (*token_pos)++;                                       \


CompNode_t* GetReturn(PARAMS_FUNC)
{
    TOKEN_NULL
    CompDump(Token, "ret", *variables);
    if (!node_is_op(Token, RETURN)) return NULL;

    CompDump(Token, "ret", *variables);
    CompNode_t* ret = Token;
    (*token_pos)++;

    CompNode_t* arg = GetVarOrNum(PARAMS_FUNC_CALL);

    if (arg == NULL)
    {
        PRINT_ERR("No-return function");
        return NULL;
    }

    ret->left = arg;
    return ret;
}

CompNode_t* GetFunction(PARAMS_FUNC)
{
    if (Token->type != FUNC_INIT) return NULL;
    CompNode_t* func_init = Token;
    (*token_pos)++;

    int size_init_var_before = init_index_var->size;

    printf(BOLD_BLUE "size_init_stack = %d\n", init_index_var->size);

    check_for(PAP_OPEN, "(");
    CompDump(Token, "token", *variables);
    CompNode_t* param = GetCommas(PARAMS_FUNC_CALL);  
    check_for(PAP_CLOSE, ")");

    CompDump(param, "param", *variables);

    printf(BOLD_BLUE "size_init_stack = %d\n", init_index_var->size);

    init_index_var->start = size_init_var_before;
    printf("\n\nstart = %d\n\n", init_index_var->start);
    
    check_for(BEGIN, "{");
    CompNode_t* main_body = GetOperation(PARAMS_FUNC_CALL);
    CompDump(main_body, "main_body", *variables);
    check_for(END, "}");

    CompDump(main_body, "func", *variables);

    int size_init_var_after = init_index_var->size;
    
    int count_init_var = size_init_var_after - size_init_var_before;
    
    printf(BOLD_BLUE "size_init_stack = %d\n", init_index_var->size);

    for (int count = 0; count < count_init_var; count++)
    {
        int elem = 0;
        INTPOP(*init_index_var, elem);
    }

    printf(BOLD_BLUE "size_init_stack = %d\n" RESET, init_index_var->size);

    init_index_var->start = 0;

    func_init->left = param;
    func_init->right = main_body;

    return func_init;
}

CompNode_t* GetCommas(PARAMS_FUNC)
{
    CompNode_t* param = GetVariable(PARAMS_FUNC_CALL);
    CompNode_t* sep   = Token;

    
    if (node_is_op(sep, COMMA))
    {
        sep->left = param; 
        (*token_pos)++;
        sep->right = GetCommas(PARAMS_FUNC_CALL);
        return sep;
    }

    return param;
}

CompNode_t* GetPrint(PARAMS_FUNC)
{
    TOKEN_NULL

    if (!node_is_op(Token, PRINT)) return NULL;

    CompNode_t* command = Token;
    (*token_pos)++;

    check_for(PAP_OPEN, "(");
    CompNode_t* arg = GetVarOrNum(PARAMS_FUNC_CALL);
    check_for(PAP_CLOSE, ")");

    command->left = arg;

    return command;
}

CompNode_t* GetIf(PARAMS_FUNC)
{
    TOKEN_NULL
    
    if (!(node_is_op(Token, IF) || node_is_op(Token, WHILE))) return NULL;
    CompNode_t* if_node = Token;
    (*token_pos)++;

    check_for(PAP_OPEN, "(");
    CompNode_t* condition = GetExpression(PARAMS_FUNC_CALL);
    check_for(PAP_CLOSE, ")");

    // CompDump(condition, "condition", variables);
    $

    check_for(BEGIN, "{");
    CompNode_t* main_body = GetOperation(PARAMS_FUNC_CALL);
    // CompDump(main_body, "main_body");
    $
    check_for(END, "}");
    
    if_node->left = condition;
    if_node->right = main_body;

    return if_node;
}

CompNode_t* GetEquat(PARAMS_FUNC)
{
    TOKEN_NULL
    
    CompNode_t* var = GetVariable(PARAMS_FUNC_CALL);
    
    if ((var == NULL) || !node_is_op(Token, EQ)) 
    {
        // (*token_pos)--;
        return GetExpression(PARAMS_FUNC_CALL);
    }

    CompNode_t* eq = Token;
    (*token_pos)++;
    
    CompNode_t* expr = GetExpression(PARAMS_FUNC_CALL);
    
    // Незаконная замена!!!!!!!!!!
    eq->left = var;
    eq->right = expr;
    
    return eq;
}

CompNode_t* GetExpression(PARAMS_FUNC)
{
    $
    TOKEN_NULL
    CompNode_t* expr1 = GetMul(PARAMS_FUNC_CALL);

    // возможны проблемы с тем, что там не степень (оператор)

    if (Token == NULL) return expr1;
    $
    if (Token->type != OP) return expr1;

    while (node_is_op(Token, ADD) || node_is_op(Token, SUB))
    {
        
        printf("я в плюсе\n");

        CompNode_t* op = Token;
        (*token_pos)++;
        $
        CompNode_t* expr2 = GetMul(PARAMS_FUNC_CALL);
        $
        // (*token_pos)++;

        op->left  = expr1;
        op->right = expr2;
        

        if (op->left != NULL)
            op->left->parent = op;

        if (op->right != NULL)
            op->right->parent = op;

        expr1 = op;
    }

    printf("Token = %p\n", Token);
    return expr1;
}

CompNode_t* GetMul(PARAMS_FUNC)
{
    TOKEN_NULL
    
    CompNode_t* expr1 = GetDegree(PARAMS_FUNC_CALL);
    
    // (*token_pos)++; 
    
    // возможны проблемы с тем, что там не степень (оператор)

    if (Token == NULL) return expr1;

    // printf("[%p]\n", Token);
    // fflush(stdout);
    $
    if (Token->type != OP) return expr1;

    while (Token->value.oper == MUL || Token->value.oper == DIV)
    {
        CompNode_t* op = Token;
        
        (*token_pos)++;
        
        CompNode_t* expr2 = GetDegree(PARAMS_FUNC_CALL);
        
        // (*tokens_pos)++

        op->left  = expr1;
        op->right = expr2;
        
        op->left->parent = op;
        op->right->parent = op;

        expr1 = op;
    }

    return expr1;
}

CompNode_t* GetDegree(PARAMS_FUNC)
{
    TOKEN_NULL
    
    CompNode_t* expr = GetCommand(PARAMS_FUNC_CALL);
    
    // (*token_pos)++;
    // возможны проблемы с тем, что там не степень (оператор)

    if (Token == NULL) return expr;
    $
    if (Token->type != OP) return expr;

    while (Token->value.oper == DEG)
    {
        CompNode_t* op = Token;
        
        (*token_pos)++;
        
        CompNode_t* deg_node = GetCommand(PARAMS_FUNC_CALL);
        
        op->left  = expr;
        op->right = deg_node;
        
        op->left->parent = op;
        op->right->parent = op;

        expr = op;
    }

    return expr;
}

CompNode_t* GetCommand(PARAMS_FUNC)
{
    TOKEN_NULL

    if ((Token->type != FUNC) && !ifTokenMath(Token)) 
    {
        return GetPermissionExp(PARAMS_FUNC_CALL);
    }

    CompNode_t* command = Token;
    (*token_pos)++;

    CompNode_t* argument = GetPermissionExp(PARAMS_FUNC_CALL);
    command->left = argument;
    command->left->parent = command;

    return command;
}

CompNode_t* GetMathCommand(PARAMS_FUNC)
{
    TOKEN_NULL

    if (ifTokenMath(Token))
    {
        CompNode_t* command = Token;
        printf("command = [%p]\n", command);
        (*token_pos)++;
        $
        CompNode_t* argument = GetPermissionExp(PARAMS_FUNC_CALL);
        printf("argument[%p]\n", argument);
        command->left = argument;
        command->left->parent = command;
        $
        return command;
    }
    else
    {
        return GetPermissionExp(PARAMS_FUNC_CALL);
    }
}

int ifTokenMath(CompNode_t* token)
{
    if (token->type != OP)
    return NO;

    if (DEG < token->value.oper && token->value.oper <= TANH) return YES;

    return NO;
}

CompNode_t* GetPermissionExp(PARAMS_FUNC) // переделать название
{
    TOKEN_NULL

    if (Token->type == OP)
    {
        if (Token->value.oper == PAP_OPEN)
        {
            
            // free(Token);
            (*token_pos)++; // пропуск PAP_OPEN
            $
            CompNode_t* node = GetExpression(PARAMS_FUNC_CALL); // сделать норм проверку
            $
            if (Token->value.oper != PAP_CLOSE) //  
            {
                PRINT_ERR("Syntax error \")\"");
                return NULL;
            }
            $
            // free(Token);
            (*token_pos)++;
            
            return node;
        }
        else
        {
            // return GetExpression(PARAMS_FUNC_CALL);
            return NULL;
        }
    }
    else
    {
        $
        return GetVarOrNum(PARAMS_FUNC_CALL);
    }
}

CompNode_t* GetVarOrNum(PARAMS_FUNC)
{
    TOKEN_NULL
    else if (Token->type == NUM) return GetNumber   (PARAMS_FUNC_CALL);
    else                         return GetVariable (PARAMS_FUNC_CALL);
}

CompNode_t* GetVariable(PARAMS_FUNC)
{
    TOKEN_NULL
    $
    // CompDump(Token, "var");

    // if (Token->type = VAR_INIT)
    // {
    //     PUSH()
    // }

    if (Token->type == VAR_INIT) INTPUSH(*init_index_var, Token->value.index_var);

    if (Token->type == VAR)
    {
        if (!var_init(Token->value.var, init_index_var, *variables)) 
        {
            $
            PRINT_ERR("Uninitialized variable");
            return NULL;
        }
    }

    if (Token->type == VAR || Token->type == VAR_INIT)
    {
        return tokens->data[(*token_pos)++];
    }

    // PRINT_ERR("It isn't variable");
    return NULL;
} 

CompNode_t* GetNumber(PARAMS_FUNC)
{
    TOKEN_NULL
    if (Token->type == NUM)
    {
        // CompDump(node);
        return tokens->data[(*token_pos)++];
    }
 
    PRINT_ERR("It isn't number");
    return NULL;
}

void PrintToken(CompNode_t* node, Stack_t variables)
{
    switch (node->type)
    {
        case OP:
            printf("operator %d\n", node->value.oper);

        case NUM:
            printf("num %d\n", node->value.num);

        case VAR:
            printf("var %s\n", variables.data[node->value.index_var]);
    }
}

int node_is_op(CompNode_t* node, Operator_val_t val)
{
    if (node == NULL) return NO;
    
    if (node->type != OP) return NO;

    if (node->value.oper != val) return NO;

    return YES;
}

int var_init(char* var, StackInt_t* init_index_var, Stack_t variables)
{
    printf(BOLD_GREEN "\n start = %d\n" RESET, init_index_var->start);
    printf(BOLD_GREEN "\n size = %d\n" RESET, init_index_var->size);
    
    StackIntPrint(init_index_var);
    printf("var = %s\n", var);

    for (int index = init_index_var->start; index < init_index_var->size; index++)
    {
        if (strcmp(var, variables.data[init_index_var->data[index]]) == 0)
            return YES;
    }

    return NO;
}

int isfunction(char* name, Stack_t functions)
{
    for (int index_func = 0; index_func < functions.size; index_func++)
    {
        if (strcmp(name, functions.data[index_func]) == 0)
            return index_func;        
    }

    return -1;
}