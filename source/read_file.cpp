#include "read_file.h"

int ifTokenMath(CompNode_t* token);

// положение токена
// логичсеская часть

// 

// DiffNode_t* GetNumber     (const char** s);
// DiffNode_t* GetP          (const char** s);
// DiffNode_t* GetCommand    (const char** s);
// DiffNode_t* GetDeg        (const char** s);
// DiffNode_t* GetT          (const char** s);
// DiffNode_t* GetExpression (const char** s);
// DiffNode_t* GetG          (const char** s);

// можно добавить в type OP, а еще будут правила для разных операторов
// разделять математические операторы и синтаксические(таких пока нет) 


#define aa printf("%s:%d [%d]push node = [%p]\n", __FILE__, __LINE__, tokens->size, node);
#define $ printf("%s:%d [%d]token = [%p]\n", __FILE__, __LINE__, *token_pos, Token); 

#define Token tokens->data[*token_pos]
#define TOKEN_NULL if (Token == NULL) return NULL;

size_t GetLex(const char* s, StackTok_t* tokens, Stack_t* variables)
{
    size_t count_lex = 0;
    
    CompNode_t* node = NULL;

    while (*s != '\0')
    {
        if (*s == '(')
        {
            node = CompNodeOPCtor(PAP_OPEN);
            aa
            fflush(stdout);

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

        if (*s == '=')
        {
            node = CompNodeOPCtor(EQ);
            TOKPUSH(*tokens, node);

            s++;
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
            printf(BOLD_BLUE "NUM_NODE = [%p]\n" RESET, node);
            TOKPUSH(*tokens, node);
            aa

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
            int var_len = 0;

            do {
                var_len++;
                s++;
            } while (isalnum(*s));

            Variable_t var = {};
            var.name_var = strndup(s - var_len, var_len);
            var.value = 0;
            
            PUSH(*variables, var);

            node = CompNodeVARCtor(variables->size - 1);
            printf(BOLD_BLUE "NODE = [%p]\n" RESET, node);
            TOKPUSH(*tokens, node);
            aa

            continue;
        }

        if (isspace(*s)) {s++; continue;}

        PRINT_ERR("Syntax_error: %c\n", *s);
    }

    return tokens->size;
}


CompNode_t* GetGeneral(StackTok_t* tokens)
{
    int token_pos = 0;
    CompNode_t* node = GetEquat(tokens, &token_pos);

    if (tokens->data[token_pos] == NULL)
    {
        return node;
    }
    
    PRINT_ERR("Syntax error");
    return NULL;
}

/// Sum -> Mul +|- Mul

// Equat -> {{VAR '='} Expression} | Expression

// x + 3 * x 

CompNode_t* GetEquat(StackTok_t* tokens, int* token_pos)
{
    CompNode_t* var = GetVariable(tokens, token_pos);

    if ((var == NULL) || !node_is_op(Token, EQ)) 
    {
        (*token_pos)--;
        return GetExpression(tokens, token_pos);
    }

    CompNode_t* eq = Token;
    (*token_pos)++;
    
    CompNode_t* expr = GetExpression(tokens, token_pos);
    
    eq->left = var;
    eq->right = expr;

    return eq;
}

CompNode_t* GetExpression(StackTok_t* tokens, int* token_pos)
{
    TOKEN_NULL
    CompNode_t* expr1 = GetMul(tokens, token_pos);

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
        CompNode_t* expr2 = GetMul(tokens, token_pos);
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

CompNode_t* GetMul(StackTok_t* tokens, int* token_pos)
{
    TOKEN_NULL
    
    CompNode_t* expr1 = GetDegree(tokens, token_pos);
    
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
        
        CompNode_t* expr2 = GetDegree(tokens, token_pos);
        
        // (*tokens_pos)++

        op->left  = expr1;
        op->right = expr2;
        
        op->left->parent = op;
        op->right->parent = op;

        expr1 = op;
    }

    return expr1;
}

CompNode_t* GetDegree(StackTok_t* tokens, int* token_pos)
{
    TOKEN_NULL
    
    CompNode_t* expr = GetMathCommand(tokens, token_pos);
    
    // (*token_pos)++;

    // возможны проблемы с тем, что там не степень (оператор)

    if (Token == NULL) return expr;
    $
    if (Token->type != OP) return expr;

    while (Token->value.oper == DEG)
    {
        CompNode_t* op = Token;
        
        (*token_pos)++;
        
        CompNode_t* deg_node = GetMathCommand(tokens, token_pos);
        
        op->left  = expr;
        op->right = deg_node;
        
        op->left->parent = op;
        op->right->parent = op;

        expr = op;
    }

    return expr;
}

CompNode_t* GetMathCommand(StackTok_t* tokens, int* token_pos)
{
    TOKEN_NULL

    if (ifTokenMath(Token))
    {
        CompNode_t* command = Token;
        printf("command = [%p]\n", command);
        (*token_pos)++;
        $
        CompNode_t* argument = GetPermissionExp(tokens, token_pos);
        printf("argument[%p]\n", argument);
        command->left = argument;
        command->left->parent = command;
        $
        return command;
    }
    else
    {
        return GetPermissionExp(tokens, token_pos);
    }
}

int ifTokenMath(CompNode_t* token)
{
    if (token->type != OP)
    return NO;

    if (DEG < token->value.oper && token->value.oper <= TANH) return YES;

    return NO;
}

CompNode_t* GetPermissionExp(StackTok_t* tokens, int* token_pos) // переделать название
{
    TOKEN_NULL

    if (Token->type == OP)
    {
        if (Token->value.oper == PAP_OPEN)
        {
            
            // free(Token);
            (*token_pos)++; // пропуск PAP_OPEN
            $
            CompNode_t* node = GetExpression(tokens, token_pos); // сделать норм проверку
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
            // return GetExpression(tokens, token_pos);
            return NULL;
        }
    }
    else
    {
        $
        return GetVarOrNum(tokens, token_pos);
    }
}

CompNode_t* GetVarOrNum(StackTok_t* tokens, int* token_pos)
{
    TOKEN_NULL
    else if (Token->type == NUM) return GetNumber   (tokens, token_pos);
    else                         return GetVariable (tokens, token_pos);
}

CompNode_t* GetVariable(StackTok_t* tokens, int* token_pos)
{
    TOKEN_NULL
    $
    if (tokens->data[*token_pos]->type == VAR)
    {
        return tokens->data[(*token_pos)++];
    }

    PRINT_ERR("It isn't variable");
    return NULL;
}

CompNode_t* GetNumber(StackTok_t* tokens, int* token_pos)
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

// syntax context
// массив, 