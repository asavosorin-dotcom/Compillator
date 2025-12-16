#include "code_tree.h"

Operator_val_t GetOP    (int* pos, char* buffer);
Type_t         GetType  (int* pos, char* buffer);
char*          GetString(int* pos, char* buffer);
int            GetNumber(int* pos, char* buffer);

void CompPrintNode(const CompNode_t* node, FILE* file_print, Stack_t* variables, Stack_t* functions)
{
    #define PRINT_FILE(...) fprintf(file_print, __VA_ARGS__)
    PRINT_FILE("(");

    PRINT_FILE("\"%s ", arr_types[node->type].name);

    switch (node->type)
    {
    case NUM:
        PRINT_FILE("%d\"", node->value.num);
        break;
    
    case VAR:
        PRINT_FILE("%s\"", node->value.var);
        break;

    case OP:
        PRINT_FILE("%s\"", arr_operators[node->value.oper].name);
        break;

    case VAR_INIT:
        PRINT_FILE("%s\"", variables->data[node->value.index_var]);
        break;

    case FUNC:
   [[fallthrow]] 
    case FUNC_INIT:
        PRINT_FILE("%s\"", functions->data[node->value.index_var]);

    default:
        break;
    }

    if (node->left)
        CompPrintNode(node->left, file_print, variables, functions);
    else
        PRINT_FILE("nil");

    if (node->right)
        CompPrintNode(node->right, file_print, variables, functions);
    else
        PRINT_FILE("nil");

    PRINT_FILE(")");

    fflush(file_print);

    #undef PRINT_FILE
}

CompNode_t* ReadNodeBack(char* buffer, Stack_t* variables, Stack_t* functions)
{
    // printf(BOLD_BLUE "In beginnig read: [%s]\n" RESET, buffer + *pos);
    static int pos = 0;

    printf("pos = [%d]\n", pos);

    pos += skip_space(buffer + pos);

    if (buffer[pos] == '(')
    {
        pos++; // пропуск скобки

        CompNode_t* node = CompNodeCtor(GetType(&pos, buffer));

        Value_t val = {};
        
        switch (node->type)
        {
            case NUM:
                val.num = GetNumber(&pos, buffer);
                break;

            case OP:
                val.oper = GetOP(&pos, buffer);
                break;

            case FUNC:
            [[fallthrough]]
            case VAR:
                val.var = GetString(&pos, buffer);
                break;
                 
                case VAR_INIT:
                 printf(BOLD_RED "Entered VAR_INIT case\n");
                {
                    char* string = GetString(&pos, buffer);
                    printf(BOLD_GREEN "string: %s\n", string);
                    fflush(stdout);
                    PUSH(*variables, string);
                    val.index_var = variables->size - 1;
                    break;
                }

            case FUNC_INIT:
                {
                    char* string = GetString(&pos, buffer);
                    PUSH(*functions, string);
                    val.index_var = functions->size - 1;
                    break;
                }

            default:
                PRINT_ERR("Unknown code %d\n", node->type);                
        }

        node->value = val;

        pos += skip_space(buffer + pos);

        node->left = ReadNodeBack(buffer, variables, functions);
        
        // if (node->left != NULL)
        // {
        //     node->left->parent = node;
        // }        

        node->right = ReadNodeBack(buffer, variables, functions);
        
        // if (node->right != NULL)
        // {
        //     node->right->parent = node; 
        //     node->right->yes_no = FLAG_NO;
        // }
        
        pos += skip_space(buffer + pos);

        if (buffer[pos] == ')')
            (pos)++;
            
        printf("[%s]\n", buffer + pos);
        return node;
    }
    else if (buffer[pos] == 'n' && buffer[pos + 1] == 'i' && buffer[pos + 2] == 'l')
    {
        pos += skip_space(buffer + pos);
        pos += strlen("nil");
        printf("nil\n");
        // printf("if nil [%s]\n", buffer + *pos);
        return NULL;
    }
    else 
    {
        printf("[%s]\n", buffer);
        // printf(RED "ERROR format code tree\n" RESET);
        return NULL;
    }
    // return ;
}

CompNode_t* ReadNode(char* buffer)
{
    // printf(BOLD_BLUE "In beginnig read: [%s]\n" RESET, buffer + *pos);
    static int pos = 0;

    printf("pos = [%d]\n", pos);

    pos += skip_space(buffer + pos);

    if (buffer[pos] == '(')
    {
        pos++; // пропуск скобки

        CompNode_t* node = CompNodeCtor(GetType(&pos, buffer));

        Value_t val = {};

        switch (node->type)
        {
            case NUM:
                val.num = GetNumber(&pos, buffer);
                break;

            case OP:
                val.oper = GetOP(&pos, buffer);
                break;

            case VAR:
            [[fallthrough]]
            case VAR_INIT:
            [[fallthrough]]
            case FUNC:
            [[fallthrough]]
            case FUNC_INIT:
                val.var = GetString(&pos, buffer);
                break;

            default:
                PRINT_ERR("Unknown code %d\n", node->type);                
        }

        node->value = val;

        pos += skip_space(buffer + pos);

        node->left = ReadNode(buffer);
        
        // if (node->left != NULL)
        // {
        //     node->left->parent = node;
        // }        

        node->right = ReadNode(buffer);
        
        // if (node->right != NULL)
        // {
        //     node->right->parent = node; 
        //     node->right->yes_no = FLAG_NO;
        // }
        
        pos += skip_space(buffer + pos);

        if (buffer[pos] == ')')
            (pos)++;
            
        printf("[%s]\n", buffer + pos);
        return node;
    }
    else if (buffer[pos] == 'n' && buffer[pos + 1] == 'i' && buffer[pos + 2] == 'l')
    {
        pos += skip_space(buffer + pos);
        pos += strlen("nil");
        printf("nil\n");
        // printf("if nil [%s]\n", buffer + *pos);
        return NULL;
    }
    else 
    {
        printf("[%s]\n", buffer);
        // printf(RED "ERROR format code tree\n" RESET);
        return NULL;
    }
    // return ;
}

// CompNode_t* ReadNode(char* buffer)
// {
//     // printf(BOLD_BLUE "In beginnig read: [%s]\n" RESET, buffer + *pos);
//     static int pos = 0;

//     printf("pos = [%d]\n", pos);

//     pos += skip_space(buffer + pos);

//     if (buffer[pos] == '(')
//     {
//         pos++; // пропуск скобки

//         CompNode_t* node = CompNodeCtor(GetType(&pos, buffer));

//         Value_t val = {};

//         switch (node->type)
//         {
//             case NUM:
//                 val.num = GetNumber(&pos, buffer);
//                 break;

//             case OP:
//                 val.oper = GetOP(&pos, buffer);
//                 break;

//             case VAR:
//             [[fallthrough]]
//             case VAR_INIT:
//             [[fallthrough]]
//             case FUNC:
//             [[fallthrough]]
//             case FUNC_INIT:
//                 val.var = GetString(&pos, buffer);
//                 break;

//             default:
//                 PRINT_ERR("Unknown code %d\n", node->type);                
//         }

//         node->value = val;

//         pos += skip_space(buffer + pos);

//         node->left = ReadNode(buffer);
        
//         // if (node->left != NULL)
//         // {
//         //     node->left->parent = node;
//         // }        

//         node->right = ReadNode(buffer);
        
//         // if (node->right != NULL)
//         // {
//         //     node->right->parent = node; 
//         //     node->right->yes_no = FLAG_NO;
//         // }
        
//         pos += skip_space(buffer + pos);

//         if (buffer[pos] == ')')
//             (pos)++;
            
//         printf("[%s]\n", buffer + pos);
//         return node;
//     }
//     else if (buffer[pos] == 'n' && buffer[pos + 1] == 'i' && buffer[pos + 2] == 'l')
//     {
//         pos += skip_space(buffer + pos);
//         pos += strlen("nil");
//         printf("nil\n");
//         // printf("if nil [%s]\n", buffer + *pos);
//         return NULL;
//     }
//     else 
//     {
//         printf("[%s]\n", buffer);
//         // printf(RED "ERROR format code tree\n" RESET);
//         return NULL;
//     }
//     // return ;
// }

void CompPrintNodeMiddle(const CompNode_t* node, FILE* file_print)
{
    #define PRINT_FILE(...) fprintf(file_print, __VA_ARGS__)
    PRINT_FILE("(");

    PRINT_FILE("\"%s ", arr_types[node->type].name);

    switch (node->type)
    {
    case NUM:
        PRINT_FILE("%d\"", node->value.num);
        break;
    
    case VAR:
        PRINT_FILE("%s\"", node->value.var);
        break;

    case OP:
        PRINT_FILE("%s\"", arr_operators[node->value.oper].name);
        break;

    case VAR_INIT:
    [[fallthrow]] 
    case FUNC:
    [[fallthrow]] 
    case FUNC_INIT:
        PRINT_FILE("%s\"", node->value.var);
        break;

    default:
        break;
    }

    if (node->left)
        CompPrintNodeMiddle(node->left, file_print);
    else
        PRINT_FILE("nil");

    if (node->right)
        CompPrintNodeMiddle(node->right, file_print);
    else
        PRINT_FILE("nil");

    PRINT_FILE(")");

    fflush(file_print);

    #undef PRINT_FILE
}

// char* Read_title(int* pos, char* buffer) // можно возвращать len и вручную изменять указатель, но потом не вызывать strlen()
// {
//     int len = 0;
//     *pos += skip_space(buffer + *pos);
//     // printf("In read title = [%s]\n", buffer + *pos);

//     sscanf(buffer + *pos, " \"%s*[^\"]\"%n", &len);
//     *(buffer + *pos + len - 1) = '\0'; // меняет вторую кавычку на 0

//     (*pos) += len;
//     // printf(CYAN "In read title = [%s]\n" RESET, buffer + *pos + 1);
//     return buffer + *pos - len + 1;
    
//     // char* point_end_name = strchr(buffer + *pos + 1, '"');
//     // *point_end_name = '\0';
    
//     // len = point_end_name - (buffer + *pos);
//     // *pos += len + 1;

//     // printf(MAGENTA "In read title = [%s]\n" RESET, buffer + *pos - len);
//     // printf(CYAN "In read title = [%s]\n" RESET, buffer + *pos + 2);

//     // return buffer + *pos - len;
// }

Type_t GetType(int* pos, char* buffer)
{
    int len = 0;

    // *pos += skip_space(buffer + *pos);

    char type_name[10] = "";
    sscanf(buffer + *pos, " \"%s %n", type_name, &len);

    // printf("len = %d\n", len);
    printf("type_name = [%s]\n", type_name);

    *pos += len;

    for (int index_type = 0; index_type < TYPE_CAPASITY; index_type++)
    {
        if (strcmp(type_name, arr_types[index_type].name) == 0) return arr_types[index_type].code;
    }


    PRINT_ERR("Unknown type [%s]\n", type_name);
    return NO_TYPE;
}

Operator_val_t GetOP(int* pos, char* buffer)
{
    int len = 0;

    // *pos += skip_space(buffer + *pos);
    
    char op_name[10] = "";
    sscanf(buffer + *pos, " %[^\"]\"%n", op_name, &len);

    *pos += len;
    printf("buffer = [%s]\n", buffer + *pos);

    printf("op_name = [%s]\n", op_name);

    for (int index_op = 0; index_op < OP_CAPASITY; index_op++)
    {
        if (strcmp(op_name, arr_operators[index_op].name) == 0)
        {
            printf("code = %d\n", arr_operators[index_op].code);
            return arr_operators[index_op].code;
        }
    }

    PRINT_ERR("Unknown op [%s]\n", op_name);
    return ADD;
}

char* GetString(int* pos, char* buffer)
{
    int len = 0;
    *pos += skip_space(buffer + *pos);
    char string[10] = ""; 

    sscanf(buffer + *pos, "%[^\"]\"%n", string, &len);

    *pos += len;

    return strdup(string);
}

int GetNumber(int* pos, char* buffer)
{
    int len = 0;
    *pos += skip_space(buffer + *pos);

    int num = 0;
    sscanf(buffer + *pos, "%d\"%n", &num, &len);

    *pos += len;

    return num;
}