#ifndef DSL_HPP_INCLUDED
#define DSL_HPP_INCLUDED

#include "diff_function.hpp" 

#define CREATE_NUM(val)       create_node(TP_NUMBER, val, NULL, NULL)

#define CREATE_VAR(val)       create_var_node(val, NULL, NULL)

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


#define IS_NODE_OP(OP) (node->type == TP_OPERATION && node->value.oper == OP)

#define IS_ZERO(node)  (node->type == TP_NUMBER && node->value.number == 0)

#define IS_ONE(node)   (node->type == TP_NUMBER && node->value.number == 1)

#define COPY_NODE(copy_node)                                        \
                node->type = copy_node->type;                       \
                node->value.oper = copy_node->value.oper;           \
                node->value.number = copy_node->value.number;       \
                node->value.var = copy_node->value.var;             \
                               

#define IS_CONST_SON_NODE(node) node->left->type == TP_NUMBER && node->right->type == TP_NUMBER


#define OP_CONST(OP)                                                        \
                node->type = TP_NUMBER;                                     \
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
                    if (node->right->value.number != 0)                     \
                        node->value.number = node->left->value.number / node->right->value.number;   \
                    break;                                                  \
                case OP_SIN:                                                \
                    break;                                                  \
                case OP_COS:                                                \
                    break;                                                  \
                case OP_LN:                                                 \
                    break;                                                  \
                case OP_DEG:                                                \
                    node->value.number = pow(node->left->value.number, node->right->value.number);   \
                    break;                                                  \
                                                                            \
                default:                                                    \
                    break;                                                  \
                }                                                           \
                node->left = NULL;                                          \
                node->right = NULL;                                         \
                
#define TOKEN_TYPE(tp) tokens->array[tokens->size].type == tp

#define TOKEN_OP(op) *tokens->array[tokens->size].elem == op
#endif

