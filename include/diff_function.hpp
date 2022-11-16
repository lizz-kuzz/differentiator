#ifndef DIFF_FUNC_HPP_INCLUDED
#define DIFF_FUNC_HPP_INCLUDED


enum TYPE_NODE {
    TP_OPERATION = 0,
    TP_VAR = 1,
    TP_NUMBER = 2,
};

enum TYPE_OPERATION {
    ADD = 0,
    SUB = 1,
    DIV = 2, 
    MUL = 3,
};

struct Node {
    TYPE_NODE       type_node;
    TYPE_OPERATION  op_value;
    double          dbl_value;
    const char *    var_value;
    Node *          left;
    Node *          right;
};

struct Tree {
    Node *root_tree;
};

typedef char * tree_elem_t; 

void printf_tree(Node *node);
Node *create_tree_from_text(Node *node, char **text_buf);
void create_tree(const char *FILE_INPUT, Tree *tree);
Node *tree_add_elem(Node *node, tree_elem_t elem);
void close_file();
#endif
