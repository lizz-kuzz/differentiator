#include "../include/diff_function.hpp"
#include "../include/file.hpp"



const char *NAME_GRAPH_FILE = "/mnt/c/Users/User/Desktop/programs/differentiator/res/graph.dot";

const char *NAME_FILE = "/mnt/c/Users/User/Desktop/programs/differentiator/res/text_output.txt";
FILE *file_tree = fopen(NAME_FILE, "w");

const int MAX_SIZE = 10;

void ctor_tree(const char *FILE_INPUT, Tree *tree) {
    char *text_buf;

    text_buf = read_file(FILE_INPUT, text_buf);

    char *point_text_buf = text_buf;

    tree->root_tree = (Node *) calloc(1, sizeof(Node));

    tree->root_tree = create_tree_from_text(tree->root_tree, &point_text_buf);

    free(text_buf);
}

Node *create_tree_from_text(Node *node, char **text_buf) {
    if (**text_buf == '\0') return node;


    if (**text_buf == '(') {
        node->left = (Node *) calloc(1, sizeof(Node));
        (*text_buf)++;
        node->left = create_tree_from_text(node->left, text_buf);
    }

    char *elem = (char *) calloc(MAX_SIZE, sizeof(char));
    char *point = elem;

    for (; **text_buf != ')' && **text_buf != '(' && **text_buf != '\0' && **text_buf != '\n'; (*text_buf)++, elem++) *elem = **text_buf; 
    elem = point;

    node = tree_add_elem(node, elem);

    if (**text_buf == '(') { 
        (*text_buf)++;
        node->right = (Node *) calloc(1, sizeof(Node));
        node->right = create_tree_from_text(node->right, text_buf);
    }

    if (**text_buf == ')') {
        (*text_buf)++; 
        return node;
    }
  
    return node;
}

Node *tree_add_elem(Node *node, char *elem) {

    assert(node && "null node");

    if (isdigit(*elem)) {
        node->type_node = TP_NUMBER;
        // double 
        // sscanf("2", "%f", node->dbl_value);
        // printf("я dbl\n");

        node->dbl_value = atoi(elem); //придумать че тут делать с даблами
    } else {
        switch (*elem) {
            case OP_ADD: {
                node->type_node = TP_OPERATION;
                node->op_value  = OP_ADD;
                break;
            } case OP_SUB: {
                node->type_node = TP_OPERATION;
                node->op_value  = OP_SUB;
                break;
            } case OP_DIV: {
                node->type_node = TP_OPERATION;
                node->op_value  = OP_DIV;
                break;
            } case OP_MUL: {
                node->type_node = TP_OPERATION;
                node->op_value  = OP_MUL;
                break;
            } case OP_DEG: {
                node->type_node = TP_OPERATION;
                node->op_value  = OP_DEG;
                break;
            } case ('s'): { //115
                node->type_node = TP_OPERATION;
                node->op_value  = OP_SIN;
                node->left = NULL;
                break;
            } case ('c'): { //99
                node->type_node = TP_OPERATION;
                node->op_value  = OP_COS;
                node->left = NULL;
                break;            
            } case ('l'): { 
                node->type_node = TP_OPERATION;
                node->op_value  = OP_LN;
                node->left = NULL;
                break;
            } default: {
                node->type_node = TP_VAR;
                node->var_value = (char *) calloc(MAX_SIZE, sizeof(char));
                node->var_value = strcpy(node->var_value, elem);
                break;
            }
        }
    }
    return node;
}

void dtor_tree(Node *node) {

    if (!node) return;

    dtor_tree(node->left);
    node->left = NULL;

    dtor_tree(node->right);
    node->right = NULL;
    free(node);
}

//--------------------------------------BEGIN TREE OUNPUT--------------------------------------------------------

void printf_tree(Node *node) {

    if (!node) return;

    if (node->left) {
        fprintf(file_tree, "(");
        printf_tree(node->left);
    }

    // if (!node->left && !node->right) {
    //     fprintf(file_tree, "(");
    //     print_node(file_tree, node);
    //     fprintf(file_tree, ")");
    // } else {
        print_node(file_tree, node);
    // }
    
    if (node->right) {
        printf_tree(node->right);
        fprintf(file_tree, ")");
    }
}


void print_node(FILE *file, Node *node) {
    assert(node && "node null");

    switch (node->type_node) {
        case TP_OPERATION:
            switch (node->op_value) {
                case OP_SIN:
                    fprintf(file, "%s", "sin");
                    break;

                case OP_COS:
                    fprintf(file, "%s", "cos");
                    break;

                case OP_LN:
                    fprintf(file, "%s", "ln");
                    // printf("i'm ln\n");
                    break;
            
                default:
                    fprintf(file, "%c", node->op_value);
                    // printf("i'm oper\n");
                    break;
            }
            break;

        case TP_VAR:
            fprintf(file, "%s", node->var_value);
            // printf("i'm var\n");
            break;

        case TP_NUMBER:
            // printf("i'm num\n");
            fprintf(file, "%g", node->dbl_value);
            break;

        default:
            break;
    }
}

void close_file() {
    fclose(file_tree);
}

static int count_png = 0;

void dump_tree(Node *root) {

    assert(root != nullptr && "null pointer tree");
    FILE *dot_file = fopen(NAME_GRAPH_FILE, "w");

    fprintf(dot_file,"digraph {\n");

    graph_dump(dot_file, root, root->left);
    graph_dump(dot_file, root, root->right);

    fprintf(dot_file,"}\n");

    fclose(dot_file);

    const int size_cmd = 100; 
    char cmd[size_cmd] = "";
    
    sprintf(cmd, "dot res/graph.dot -Tpng -o res/output%d.png 2>text_error.txt", count_png);
    count_png++;
    system(cmd);
    
}


void graph_dump(FILE *dot_file, Node *node, Node *node_son) {
    
    if (!node_son) {
        return;
    }

    fprintf   (dot_file, "\tnode%p[label=\"", node);
    print_node(dot_file, node);
    fprintf   (dot_file, "\"]\n");

    fprintf   (dot_file, "\tnode%p[label=\"", node_son);
    print_node(dot_file, node_son);
    fprintf   (dot_file, "\"]\n");
    
    fprintf(dot_file, "\tnode%p -> node%p\n", node, node_son);

    graph_dump(dot_file, node_son, node_son->left);
    graph_dump(dot_file, node_son, node_son->right);

}

//--------------------------------------END TREE OUNPUT--------------------------------------------------------


//------------------------------------DIFFERENTIATOR-----------------------------------------------------------


Node *copy_tree(Node *node) {

    if (!node) return node;

    Node *copy_node = (Node *) calloc(1, sizeof(Node));

    copy_node->left      = node->left;
    copy_node->right     = node->right;
    copy_node->dbl_value = node->dbl_value;
    copy_node->op_value  = node->op_value;
    copy_node->right     = node->right;
    copy_node->type_node = node->type_node;
    copy_node->var_value = node->var_value;

    if (node->left) {
        copy_node->left = copy_tree(copy_node->left);
    }

    if (node->right) {
        copy_node->right = copy_tree(copy_node->right);
    }

    return copy_node;

}

Node *create_node(TYPE_NODE tp_node, int value, Node *node_left, Node *node_right) {
    
    Node *node = (Node *)calloc(1, sizeof(Node));

    node->type_node = tp_node;
    node->left = node_left;
    node->right = node_right;

    switch (tp_node) {
        case TP_NUMBER:
            node->dbl_value = value;
            break;

        case TP_OPERATION:
            node->op_value = (TYPE_OPERATION)value;
            break;

        default:
            break;
    }
    return node;
}

Node *diff_tree(Node *node) {

    switch (node->type_node) {
        case TP_NUMBER: return CREATE_NUM(0);

        case TP_VAR:    return CREATE_NUM(1);

        case TP_OPERATION: {
            switch (node->op_value){
                case OP_ADD: 
                    return ADD(dL, dR);

                case OP_SUB: 
                    return SUB(dL, dR);

                case OP_MUL:
                    return ADD(MUL(dL, cR), MUL(cL, dR));

                case OP_DIV: 
                    return DIV(SUB(MUL(dL, cR), MUL(cL, dR)), MUL(cR, cR));
                
                case OP_SIN:
                    return MUL(COS(cR), dR);
                
                case OP_COS:
                    return MUL(MUL(SIN(cR), CREATE_NUM(-1)), dR);
                
                case OP_DEG:
                    if (node->left->type_node == TP_NUMBER) {
                        return MUL(DEG(cL, cR), MUL(dR, LN(cL)));
                    } else {
                        return MUL(DEG(cL, SUB(cR, CREATE_NUM(1))), dL);
                    }

                case OP_LN:
                    if (node->right->type_node == TP_NUMBER) {
                        return CREATE_NUM(0);
                    } else {
                        return MUL(DIV(CREATE_NUM(1), cR), dR);
                    }
                default:
                    break;
            }
        }
    
        default:
            return node;
            break;
    }
}
