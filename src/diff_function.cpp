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
        node->tp_node = TP_NUMBER;
        node->value.number = atoi(elem); 
    } else {
        switch (*elem) {
            case OP_ADD: {
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_ADD;
                break;
            } case OP_SUB: {
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_SUB;
                break;
            } case OP_DIV: {
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_DIV;
                break;
            } case OP_MUL: {
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_MUL;
                break;
            } case OP_DEG: {
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_DEG;
                break;
            } case (OP_SIN): { //115
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_SIN;
                node->left = NULL;
                break;
            } case (OP_COS): { //99
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_COS;
                node->left = NULL;
                break;            
            } case (OP_LN): { 
                node->tp_node = TP_OPERATION;
                node->value.oper  = OP_LN;
                node->left = NULL;
                break;
            } default: {
                node->tp_node = TP_VAR;
                node->value.var = (char *) calloc(MAX_SIZE, sizeof(char));
                node->value.var = strcpy(node->value.var, elem);
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

bool checking_for_priority(Node *node, Node *parent) {
    if (!node || !parent) return false;

    if (parent->tp_node == TP_OPERATION && node->tp_node == TP_OPERATION && 
    ((parent->value.oper == OP_ADD || parent->value.oper == OP_SUB) || (node->value.oper == parent->value.oper)))
        return true;

    else return false;
}

void print_tree(Node *node, Node *parent) {

    if (!node) return;

    if (checking_for_priority(node, parent)) {

    } else if (node->left && parent) {
        fprintf(file_tree, "(");
    }
   
    if (!(!node->left && node->right)) {
        print_tree(node->left, node);
    }

    print_node(file_tree, node);
    
    if (node->right) {

        print_tree(node->right, node);

        if (checking_for_priority(node, parent)) {

        } else 

        if (!(!node->left && node->right) && parent) {
            fprintf(file_tree, ")");
        }
        
    }
    
}


void print_node(FILE *file, Node *node) {
    assert(node && "node null");

    switch (node->tp_node) {
        case TP_OPERATION:
            switch (node->value.oper) {
                case OP_SIN:
                    fprintf(file, "sin");
                    break;

                case OP_COS:
                    fprintf(file, "cos");
                    break;

                case OP_LN:
                    fprintf(file, "ln");
                    break;
    
                case OP_ADD:
                    fprintf(file, " + ");
                    break;
                
                case OP_SUB:
                    fprintf(file, " - ");
                    break;
                
                case OP_MUL:
                    fprintf(file, " * ");
                    break;
                
                case OP_DIV:
                    fprintf(file, " / ");
                    break;

                case OP_DEG:
                    fprintf(file, " ^ ");
                    break;

                default:
                    fprintf(file, "don't find oper");
                    break;
            }
            break;

        case TP_VAR:
            fprintf(file, "%s", node->value.var);
            break;

        case TP_NUMBER:
            if (node->value.number < 0) {
                fprintf(file, "(%d)", node->value.number);
            } else {
                fprintf(file, "%d", node->value.number);
            }
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

//--------------------------------------END TREE OUNPUT----------------------------------------------------------

//----------------------------------------END TEX_DUMP-----------------------------------------------------------

void title_tex_dump(Node *node) {

    const char *FILE_TEX = "/mnt/c/Users/User/Desktop/programs/differentiator/res/tex_dump1.tex";

    FILE *file_tex = fopen(FILE_TEX, "w");

    fprintf(file_tex, "\\documentclass[12pt, letterpaper, twoside]{article}\n");
    fprintf(file_tex, "\\usepackage[T1]{fontenc}\n");
    fprintf(file_tex, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file_tex, "\\usepackage[russian]{babel}\n");

    fprintf(file_tex, "\\title{ВЗЯТИЕ ПРОИЗВОДНОЙ КРОКОДИЛА}\n");
    fprintf(file_tex, "\\author{Кузнецова Елизавета Юрьевна}\n");
    fprintf(file_tex, "\\date{22.11.2022}\n");
    

    fprintf(file_tex, "\\begin{document}\n");
    fprintf(file_tex, "\\maketitle\n");

    fprintf(file_tex, "\\newpage\n");
    fprintf(file_tex, "Подробное описание взятия производной заданной функции:\n");
    fprintf(file_tex, "\\[f(x) =");

    print_tex(file_tex, node, NULL);
    fprintf(file_tex, "\\]\n");

    fprintf(file_tex, "\\end{document}\n");
    
    fclose(file_tex);

    system("pdflatex res/tex_dump1.tex");
}

void print_tex(FILE *file_tex, Node *node, Node *parent) {

    if (!node) return;

    if (checking_for_priority(node, parent)) ;
    else if (node->left && parent)
        fprintf(file_tex, "(");
    
    if (parent && parent->value.oper == OP_DEG){
        fprintf(file_tex, "{");
    }

    if (node->value.oper == OP_DIV) {
        fprintf(file_tex, "{");
    }
    
    if (parent && parent->value.oper == OP_DIV) {
        fprintf(file_tex, "{");
    }
    
    if (!(!node->left && node->right)) {
        print_tex(file_tex, node->left, node);
    }

    print_tex_node(file_tex, node);
    
    print_tex(file_tex, node->right, node);



    if ((parent && parent->value.oper == OP_DIV)) {
        fprintf(file_tex, "}");
    } 
    
    if (node->value.oper == OP_DIV) {
        fprintf(file_tex, "}");
    }

    if (checking_for_priority(node, parent)) ;
    else if (node->right && !(!node->left && node->right) && parent) 
            fprintf(file_tex, ")");
            

    if (parent && parent->value.oper == OP_DEG) {
        fprintf(file_tex, "}");
    }
}

void print_tex_node(FILE *file_tex, Node *node) {
    assert(node && "node null");

    switch (node->tp_node) {
        case TP_OPERATION:
            switch (node->value.oper) {
                case OP_SIN:
                    fprintf(file_tex, "\\sin");
                    break;

                case OP_COS:
                    fprintf(file_tex, "\\cos");
                    break;

                case OP_LN:
                    fprintf(file_tex, "\\ln");
                    break;
    
                case OP_ADD:
                    fprintf(file_tex, " + ");
                    break;
                
                case OP_SUB:
                    fprintf(file_tex, " - ");
                    break;
                
                case OP_MUL:
                    fprintf(file_tex, " \\cdot ");
                    break;
                
                case OP_DIV:
                    fprintf(file_tex, " \\over ");
                    break;

                case OP_DEG:
                    fprintf(file_tex, "^");
                    break;

                default:
                    fprintf(file_tex, "don't find oper");
                    break;
            }
            break;

        case TP_VAR:
            fprintf(file_tex, "%s", node->value.var);
            break;

        case TP_NUMBER:
            if (node->value.number < 0) {
                fprintf(file_tex, "(%d)", node->value.number);
            } else {
                fprintf(file_tex, "%d", node->value.number);
            }
            break;

        default:
            break;
    }
}
//----------------------------------------END TEX_DUMP-----------------------------------------------------------




//------------------------------------BEGIN DIFFERENTIATOR-----------------------------------------------------------


Node *copy_tree(Node *node) {

    if (!node) return node;

    Node *copy_node = (Node *) calloc(1, sizeof(Node));

    copy_node->left      = node->left;
    copy_node->right     = node->right;
    copy_node->value     = node->value;
    copy_node->right     = node->right;
    copy_node->tp_node   = node->tp_node;

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

    node->tp_node = tp_node;
    node->left = node_left;
    node->right = node_right;

    switch (tp_node) {
        case TP_NUMBER:
            node->value.number = value;
            break;

        case TP_OPERATION:
            node->value.oper = (TYPE_OPERATION)value;
            break;

        case TP_VAR:
            break;

        default:
            break;
    }

    return node;
}

Node *diff_tree(Node *node) {
   
    switch (node->tp_node) {
        case TP_NUMBER: return CREATE_NUM(0);

        case TP_VAR:    return CREATE_NUM(1);

        case TP_OPERATION: {
            switch (node->value.oper){
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
                    if (node->left->tp_node == TP_NUMBER) {
                        return MUL(DEG(cL, cR), MUL(dR, LN(cL)));
                    } else {
                        return MUL(MUL(cR, DEG(cL, SUB(cR, CREATE_NUM(1)))), dL);
                    }

                case OP_LN:
                    if (node->right->tp_node == TP_NUMBER) {
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

//-----------------------------------END DIFFERENTIATOR-----------------------------------------------------------



//-----------------------------------------OPTIMIZER--------------------------------------------------------------
 
void optimizer_tree(Node *node) {

    int continue_optimizer = 0;

    while (true) {
        if (folding_constant(node, &continue_optimizer) == 0) {         
            break;
        }
        continue_optimizer = 0;
    }

    dump_tree(node);

}

int folding_constant(Node *node, int *continue_optimiz) {
    
    if (!node) return *continue_optimiz;

    folding_constant(node->left, continue_optimiz);

    if (node->left && node->right) {

        if (IS_NODE_OP(OP_MUL) && ((IS_ZERO(node->left)) || (IS_ZERO(node->right)))) {
            node->tp_node = TP_NUMBER;
            node->value.number = 0;
            node->left = NULL;
            node->right = NULL;
            *continue_optimiz = 1;

        } else if ((IS_NODE_OP(OP_MUL) || IS_NODE_OP(OP_DEG)) && IS_ONE(node->left)) {
            COPY_NODE(node->right);
            node->left = node->right->left;  
            node->right = node->right->right;
            *continue_optimiz = 1;       

        } else if ((IS_NODE_OP(OP_MUL) || IS_NODE_OP(OP_DEG)) && IS_ONE(node->right)) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left;  
            *continue_optimiz = 1;

        } else if ((IS_NODE_OP(OP_ADD) || IS_NODE_OP(OP_SUB)) && IS_ZERO(node->left)) {
            COPY_NODE(node->right);
            node->left = node->right->left;  
            node->right = node->right->right;
            *continue_optimiz = 1;

        } else if ((IS_NODE_OP(OP_ADD) || IS_NODE_OP(OP_SUB)) && IS_ZERO(node->right)) {
            COPY_NODE(node->left);
            node->right = node->left->right;
            node->left = node->left->left; 
            *continue_optimiz = 1;

        } else if (node->tp_node == TP_OPERATION && IS_CONST_NODE(node)) {
            OP_CONST(node->value.oper);
            *continue_optimiz = 1;
        }
    }
    folding_constant(node->right, continue_optimiz);
    return *continue_optimiz;
}

//-------------------------------END OPTIMIZER--------------------------------------------
