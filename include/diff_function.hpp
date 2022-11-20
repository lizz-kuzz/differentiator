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
union type_node {
    TYPE_OPERATION  oper;
    int             number;
    char *          var;
};

struct Node {
    TYPE_NODE       tp_node;
    type_node       value;
    // TYPE_OPERATION  op_value;
    // int             value;
    // char *          var_value;
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



#define IS_NODE_OP(OP) node->tp_node == TP_OPERATION && node->value.oper == OP

#define IS_ZERO(node) node->tp_node == TP_NUMBER && node->value.number == 0

#define IS_ONE(node) node->tp_node == TP_NUMBER && node->value.number == 1

#define COPY_NODE(copy_node)                                    \
                node->tp_node = copy_node->tp_node;         \
                node->value.oper = copy_node->value.oper;           \
                node->value.number = copy_node->value.number;                 \
                node->value.var = copy_node->value.var;         \
                               

#define IS_CONST_NODE(node) node->left->tp_node == TP_NUMBER && node->right->tp_node == TP_NUMBER

#define OP_CONST(OP)                                                         \
                node->tp_node = TP_NUMBER;                                \
                switch (OP) {                                               \
                case OP_ADD:                                                \
                    node->value.number = node->left->value.number + node->right->value.number;   \
                    break;                                                  \
                case OP_SUB:                                                \
                    node->value.number = node->left->value.number - node->right->value.number;   \
                    break;                                                  \
                case OP_MUL:                                                \
                    node->value.number = node->left->value.number * node->right->value.number;   \
                    break;                                                  \
                case OP_DIV:                                                \
                    node->value.number = node->left->value.number / node->right->value.number;   \
                    break;                                                  \
                default:                                                    \
                    break;                                                  \
                }                                                           \
                node->left = NULL;                                          \
                node->right = NULL;                                         \
                
                
                // node->value.oper = (TYPE_OPERATION)0;                         \


void optimizer_tree(Node *node);

int folding_constant(Node *node, int *continue_optimiz);

#endif
