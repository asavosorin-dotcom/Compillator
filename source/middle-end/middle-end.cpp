#include "./middle-end/middle-end.h"

static bool flag_change = 0;
int nodeisnum(CompNode_t* node, double num);
void nodenull(CompNode_t* node);
CompNode_t* cpy_left  (CompNode_t* node);
CompNode_t* cpy_right (CompNode_t* node);

CompNode_t* cpy_left(CompNode_t* node)
{
    CompNode_t* node_ret = node->left;
    // printf("free node[%p]\nfree node[%p]\n", node, node->right);
    free(node->right);             
    free(node);              
    flag_change = 1;      
    
    return node_ret;
}

CompNode_t* cpy_right(CompNode_t* node)
{
    CompNode_t* node_ret = node->right;
    // printf("free node[%p]\nfree node[%p]\n", node->left, node);
    free(node->left);             
    free(node);              
    flag_change = 1;
    
    return node_ret;
}


CompNode_t* CompOptimizConst(CompNode_t* node)
{
    // printf(":%s:%d node = [%p]\n", __FILE__, __LINE__, node);

    // CompDump(node, "before");

    fflush(stdout);

    if (node->left != NULL)
        node->left = CompOptimizConst(node->left);
    
    if (node->right != NULL)
        node->right = CompOptimizConst(node->right);
    
    if (node->left == NULL || node->right == NULL)
        return node;

    if (node->left->type == NUM && node->right->type == NUM)
    {
        flag_change = 1;
        double num = CompSolveExpresion(node);

        node->type = NUM;
        node->value = {};
        node->value.num = num;

        // printf(BOLD_RED "SADLVBHLV\n" RESET);
        // fflush(stdout);
        // printf("free node[%p]\nfree node[%p]\n", node->left, node->right);
        free(node->left);
        free(node->right);

        node->left = NULL;
        node->right = NULL;
        
        // printf(":%s:%d node->left = [%p]\n", __FILE__, __LINE__, node->left);
        // printf(":%s:%d node->right = [%p]\n", __FILE__, __LINE__, node->right);

        // CompDump(node->parent, "node opt");
    }

    // CompDump(node, "opt");

    return node;
}

CompNode_t* CompOptimizNeytralElem(CompNode_t* node)
{
    if (node->left != NULL)
        node->left = CompOptimizNeytralElem(node->left);
    
    if (node->right != NULL)
        node->right = CompOptimizNeytralElem(node->right);

    if (node->left == NULL || node->right == NULL)
        return node;

    if (node->type == OP)
    {
        switch (node->value.oper)
        {
        case MUL:
            if (nodeisnum(node->left, 0) || nodeisnum(node->right, 0))
            {
                nodenull(node);
                // CompDump(node->parent, "node null");
            }
            
            else if (nodeisnum(node->left, 1))
            {
                node = cpy_right(node);
            }

            else if (nodeisnum(node->right, 1))
            {
                node = cpy_left(node);
            }

            return node;
        
        case DIV:
            if (nodeisnum(node->left, 0))
            {
                nodenull(node);
            }

            else if (nodeisnum(node->right, 1))
            {
                node = cpy_left(node);
            }

            return node;
        
        case ADD: 
            if (nodeisnum(node->left, 0))
            {
                node = cpy_right(node);
                return node;
            }

        [[fallthrough]]

        case SUB:

            if (nodeisnum(node->right, 0))
            {                
                // printf("node[%p] before\n", node);
                node = cpy_left(node);
                // printf("node[%p] after\n", node);
            }

            return node;

        default:
            return node;
        }
    }

    return node;
}


CompNode_t* CompOptimiz(CompNode_t* root)
{
    while (1)
    {
        flag_change = 0;
        root = CompOptimizConst(root);
        root = CompOptimizNeytralElem(root);

        if (!flag_change)
            return root;
    }
}

int nodeisnum(CompNode_t* node, double num)
{
    // printf("%p\n", node);
    
    if (node->type != NUM)
        return 0;

    if (node->value.num != num)
        return 0;

    return 1;
}

void nodenull(CompNode_t* node)
{
    node->type = NUM;
    node->value = {};
    node->value.num = 0;

    // printf("free node[%p]\nfree node[%p]\n", node->left, node->right);

    CompDtor(node->left);
    CompDtor(node->right);

    node->left = NULL;
    node->right = NULL;
    
    flag_change = 1;
}

int CompSolveExpresion(CompNode_t* root)
    {
        if (root == NULL)
        {
            // PRINT_ERR("null pointer node")
            return 0;
        }
        
        switch (root->type)
        {
            case NUM:
                return root->value.num;

            case OP:
                {
                int num1 = CompSolveExpresion(root->left);
                int num2 = CompSolveExpresion(root->right);

                switch (root->value.oper)
                {
                case ADD:
                    return num1 + num2;
                
                case SUB:
                    return num1 - num2;

                case MUL:
                    return num1 * num2;

                case DIV:
                    if (num2 == 0)
                    {
                        PRINT_ERR("Division by zero");
                        return 0;
                    }
                    return num1 / num2; // 0

                case DEG:
                    return pow(num1, num2);

                case SIN:
                    return sin(num1);
            
                case COS:
                    return cos(num1);
                    
                case LN:
                    return log(num1);            

                case TAN:
                    return tan(num1);

                case ASIN:
                    return asin(num1);

                case ACOS:
                    return acos(num1);

                case ATAN:
                    return atan(num1);

                case SINH:
                    return sinh(num1);
                    
                case COSH:
                    return cosh(num1);

                case TANH:
                    return tanh(num1);

                default:
                    return 0;
                }
                break;
            }

            default:
                // printf(BOLD_RED "Type doesn't exist\n" RESET);
                return 0;
        }
    }