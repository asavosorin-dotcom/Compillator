#include "comp_dump.h"

FILE* file_htm = fopen("Logfile.htm", "w");

static int index_png = 0;

void CompDumpNode(CompNode_t* node, FILE* file_dump, Stack_t variables)
{       
    assert(node);
    
    #define PRINT_NODE_IMAGE(print_type, color, ...) PRINT_IMAGE("\tnode%p[label = \"{parent: %p | %p| TYPE: %s |VAL: " print_type " | {%p | %p}}\", shape = Mrecord, style = \"filled\", fillcolor = " color "]\n", node, node->parent, node, arr_types[node->type] , __VA_ARGS__, node->left, node->right);

    // добавить разные формы и цвета для разных типов ячеек 

    switch (node->type)
    {
        case OP:
            if (node->value.oper == SEP)
            {
                PRINT_NODE_IMAGE("%s", "\"#ff00d0ff\"", arr_operators[node->value.oper].command_name);
                break; 
            }
            PRINT_NODE_IMAGE("%s", "\"#5f5fffff\"", arr_operators[node->value.oper].command_name);
            break;
            
        case NUM:
            PRINT_NODE_IMAGE("%d", "\"#0CFF0C\"", node->value.num);
            break;

        case VAR:
            PRINT_NODE_IMAGE("(\'%s\')", "\"#FF0C0C\"", node->value.var);
            break;

        case FUNC:
            PRINT_NODE_IMAGE("(\'%s\')", "\"#e3ff0cff\"", "FUNC_NAME");
            break;
            
        case VAR_INIT:
            PRINT_NODE_IMAGE(" (\'%s\')", "\"#00ccffff\"", variables.data[node->value.index_var]);
            break;
        case FUNC_INIT:
            PRINT_NODE_IMAGE(" (\'%s\')", "\"#a600ffff\"", /*variables.data[node->value.index_var].value*//*variables.data[node->value.index_var].name_var*/ "FUNC INTT");
            break;

        default:
            printf(BOLD_RED "This type doesn't exist\n" RESET);
    }

    if (node->left != NULL) 
    {
        CompDumpNode(node->left, file_dump, variables);
        PRINT_IMAGE("\tnode%p -> node%p [color = \"blue\"]\n ", node, node->left);
    }
    
    if (node->right != NULL)
    {
        CompDumpNode(node->right, file_dump, variables);
        PRINT_IMAGE("\tnode%p -> node%p [color = \"red\"]\n", node, node->right);
    }
    
    #undef PRINT_NODE_IMAGE
}

void CompDumpImage(CompNode_t* node, Stack_t variables)
{   
    const char* filename = "Comp_dump.txt";
    FILE* file_dump = fopen(filename, "w");
    
    PRINT_IMAGE("digraph {\n");
    CompDumpNode(node, file_dump, variables);

    PRINT_IMAGE("}");
    
    char command[100] = "";
    fclose(file_dump);
    
    sprintf(command, "dot \"%s\" -T png -o pictures/graph%d.png", filename, index_png);
    // printf("command = %s\n", command);
    system(command);

    fflush(stdout);

    index_png++;
}

void CompDump(CompNode_t* node, const char* text, Stack_t variables)
{
    PRINT_HTM("<pre>\n");
    PRINT_HTM("\t<h3>DUMP %s</h3>\n", text);
    
    CompDumpImage(node, variables);
    
    PRINT_HTM("Image: \n <img src= \"pictures/graph%d.png\">", index_png - 1);
    PRINT_HTM("</pre>");
    
    fflush(file_htm);
}

