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
    OP_SIN = 'sin',
    OP_COS = 'cos',
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




#define CREATE_NUM(val) create_node(TP_NUMBER, val, NULL, NULL)

#define ADD_(node1, node2) create_node(TP_OPERATION, OP_ADD, node1, node2)

#define SUB(node1, node2) create_node(TP_OPERATION, OP_SUB, node1, node2)

#define MUL(node1, node2) create_node(TP_OPERATION, OP_MUL, node1, node2)

#define DIV(node1,node2)  create_node(TP_OPERATION, OP_DIV, node1, node2)

#define dL diff_tree(node->left)
#define dR diff_tree(node->right)
#define cL copy_tree(node->left)
#define cR copy_tree(node->right)

#define ADD(node1, node2) 

Node *diff_tree(Node *node);

Node *copy_tree(Node *node); 

Node *create_node(TYPE_NODE tp_node, int value, Node *node_left, Node *node_right);
#endif
