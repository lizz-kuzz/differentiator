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
    OP_DEG = '^',
    OP_LN  = 'ln',
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

Node *create_tree_from_text(Node *node, char **text_buf);

void ctor_tree(const char *FILE_INPUT, Tree *tree);

void dtor_tree(Node *node);

Node *tree_add_elem(Node *node, char *elem);



void dump_tree(Node *root);

void printf_tree(Node *node);

void print_node(FILE *file, Node *node);

void graph_dump(FILE *dot_file, Node *node, Node *node_son);

void close_file();



#define CREATE_NUM(val)       create_node(TP_NUMBER, val, NULL, NULL)

#define ADD(node_l, node_r)   create_node(TP_OPERATION, OP_ADD, node_l, node_r)

#define SUB(node_l, node_r)   create_node(TP_OPERATION, OP_SUB, node_l, node_r)

#define MUL(node_l, node_r)   create_node(TP_OPERATION, OP_MUL, node_l, node_r)

#define DIV(node_l, node_r)   create_node(TP_OPERATION, OP_DIV, node_l, node_r)

#define SIN(node)             create_node(TP_OPERATION, OP_SIN, NULL, node)

#define COS(node)             create_node(TP_OPERATION, OP_COS, NULL, node)

#define DEG(node_l, node_r)   create_node(TP_OPERATION, OP_DEG, node_l, node_r)

#define LN(node)              create_node(TP_OPERATION, OP_LN, NULL, node)

#define dL diff_tree(node->left)
#define dR diff_tree(node->right)
#define cL copy_tree(node->left)
#define cR copy_tree(node->right)

Node *diff_tree(Node *node);

Node *copy_tree(Node *node); 

Node *create_node(TYPE_NODE tp_node, int value, Node *node_left, Node *node_right);


#endif
