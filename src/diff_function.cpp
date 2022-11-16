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

Node *tree_add_elem(Node *node, tree_elem_t elem) {

    assert(node && "null node");

    if (isdigit(*elem)) {
        node->type_node = TP_NUMBER;
        node->dbl_value = atoi(elem); //придумать че тут делать с даблами
    } else {
        switch (*elem) {
            case OP_ADD: {
                node->type_node = TP_OPERATION;
                node->op_value = OP_ADD;
                break;
            } case OP_SUB: {
                node->type_node = TP_OPERATION;
                node->op_value = OP_SUB;
                break;
            } case OP_DIV: {
                node->type_node = TP_OPERATION;
                node->op_value = OP_DIV;
                break;
            } case OP_MUL: {
                node->type_node = TP_OPERATION;
                node->op_value = OP_MUL;
                break;
            } case OP_SIN: {
                node->type_node = TP_OPERATION;
                node->op_value = OP_SIN;
                break;
            } case OP_COS: {
                node->type_node = TP_OPERATION;
                node->op_value = OP_COS;
                break;
            } default: {
                node->type_node = TP_VAR;
                node->var_value = (tree_elem_t) calloc(MAX_SIZE, sizeof(char));
                node->var_value = strcpy(node->var_value, elem);
                break;
            }
        }
    }
    return node;
}

void dtor_tree(Node *node) {

    if (!node) return;

    if (node->type_node == TP_VAR) {
        free(node->var_value);
    }

    dtor_tree(node->left);
    node->left = NULL;

    dtor_tree(node->right);
    node->right = NULL;
    free(node);
}


void printf_tree(Node *node) {

    if (!node) return;

    if (node) {
        if (node->left) {
            fprintf(file_tree, "(");
            printf_tree(node->left);
        }

        print_node(file_tree, node);
        
        if (node->right) {
            printf_tree(node->right);
            fprintf(file_tree, ")");
        }
    }
}


void print_node(FILE *file, Node *node) {
    assert(node && "node null");

    switch (node->type_node) {
        case TP_OPERATION:
            fprintf(file, "%c", (char)node->op_value);
            break;
        case TP_VAR:
            fprintf(file, "%s", node->var_value);
            break;
        case TP_NUMBER:
            fprintf(file, "%g", node->dbl_value);
            break;
        default:
            break;
    }
}

void close_file() {
    fclose(file_tree);
}



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
    
    sprintf(cmd, "dot res/graph.dot -Tpng -o res/output.png 2>text_error.txt");

    system(cmd);
    
}


void graph_dump(FILE *dot_file, Node *node, Node *node_son) {
    
    if (!node_son) {
        return;
    }
    if (node) {
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
}