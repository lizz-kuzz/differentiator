#ifndef DIFF_FUNC_HPP_INCLUDED
#define DIFF_FUNC_HPP_INCLUDED

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

enum TYPE_NODE {
    TP_OPERATION = 1,
    TP_VAR = 2,
    TP_NUMBER = 3,
};

enum TYPE_OPERATION {
    OP_ADD = '+',
    OP_SUB = '-',
    OP_DIV = '/', 
    OP_MUL = '*',
    OP_SIN = 's',
    OP_COS = 'c',
};

struct Node {
    TYPE_NODE       type_node;
    TYPE_OPERATION  op_value;
    double          dbl_value;
    char *          var_value;
    Node *          left;
    Node *          right;
};

struct Tree {
    Node *root_tree;
};

typedef char * tree_elem_t; 

void printf_tree(Node *node);

void print_node(FILE *file, Node *node);

Node *create_tree_from_text(Node *node, char **text_buf);

void ctor_tree(const char *FILE_INPUT, Tree *tree);

void dtor_tree(Node *node);

Node *tree_add_elem(Node *node, tree_elem_t elem);

void close_file();

void dump_tree(Node *root);

void graph_dump(FILE *dot_file, Node *node, Node *node_son);

#endif
