#include "../include/diff_function.hpp"
#include "../include/DSL.hpp"
#include "../include/file.hpp"


// -------------------------------------BEGIN TREE FUNCTIONS--------------------------------------

const char *NAME_GRAPH_FILE = "res/graph.dot";
const char *NAME_FILE =       "res/text_output.txt";
FILE *file_tree = fopen(NAME_FILE, "w");

const int MAX_SIZE = 10;

void ctor_tree(const char *FILE_INPUT, Tree *tree) {
    char *text_buf;

    text_buf = read_file(FILE_INPUT, text_buf);
    printf("%s\n", text_buf);
    char *point_text_buf = text_buf;
    Tokenizer tokens = {};
    tokenizer_ctor(&tokens, point_text_buf);

    for (int i = 0; i < tokens.capacity; i++) {
        printf("%d [%s] ", tokens.array[i].type, tokens.array[i].elem);
    }
    printf("\n");

    tree->root_tree = (Node *) calloc(1, sizeof(Node));

    tree->root_tree = get_g(&tokens);

    tokenizer_dtor(&tokens);

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
        node->type = TP_NUMBER;
        node->value.number = atoi(elem); 
    } else {
        switch (*elem) {
            case OP_ADD: {
                node->type = TP_OPERATION;
                node->value.oper  = OP_ADD;
                break;
            } case OP_SUB: {
                node->type = TP_OPERATION;
                node->value.oper  = OP_SUB;
                break;
            } case OP_DIV: {
                node->type = TP_OPERATION;
                node->value.oper  = OP_DIV;
                break;
            } case OP_MUL: {
                node->type = TP_OPERATION;
                node->value.oper  = OP_MUL;
                break;
            } case OP_DEG: {
                node->type = TP_OPERATION;
                node->value.oper  = OP_DEG;
                break;
            } case (OP_SIN): { //115
                node->type = TP_OPERATION;
                node->value.oper  = OP_SIN;
                node->left = NULL;
                break;
            } case (OP_COS): { //99
                node->type = TP_OPERATION;
                node->value.oper  = OP_COS;
                node->left = NULL;
                break;            
            } case (OP_LN): { 
                node->type = TP_OPERATION;
                node->value.oper  = OP_LN;
                node->left = NULL;
                break;
            } default: {
                node->type = TP_VAR;
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
// -------------------------------------END TREE FUNCTIONS------------------------------------------------------------


// -------------------------------------BEGIN RECURSIVE DESCENT-------------------------------------------------------

Node *get_g(Tokenizer *tokens) {
    Node *node = get_add_sub(tokens);

    assert(tokens->size == tokens->capacity);

    return node;
}

Node *get_add_sub(Tokenizer *tokens) {
    Node *node_r = get_mul_div(tokens);
   
    while (TOKEN_TYPE(TP_OPERATION) && (TOKEN_OP(OP_ADD) || TOKEN_OP(OP_SUB))) {

        char op = *tokens->array[tokens->size].elem;

        tokens->size++;

        Node *node_l = get_mul_div(tokens);

        if (op == OP_ADD) {
            node_r = ADD(node_r, node_l);
        } else if (op == OP_SUB) {
            node_r = SUB(node_r, node_l);
        }
    }

    return node_r; 
}


Node *get_mul_div(Tokenizer *tokens) {

    Node *node_r = get_deg(tokens);

    while (TOKEN_TYPE(TP_OPERATION) && (TOKEN_OP(OP_MUL) || TOKEN_OP(OP_DIV))) {
        char op = *tokens->array[tokens->size].elem;

        tokens->size++;

        Node *node_l = get_deg(tokens);
        
        if (op == OP_MUL) {
            node_r = MUL(node_r, node_l);
        } else if (op == OP_DIV) {
            node_r = DIV(node_r, node_l);
        }
    }

    return node_r;
}

Node *get_deg(Tokenizer *tokens) {

    Node *node_r = get_unary_op(tokens);
 
    while (TOKEN_TYPE(TP_OPERATION) && TOKEN_OP(OP_DEG)) {

        char op = *tokens->array[tokens->size].elem;
        tokens->size++;

        Node *node_l = get_unary_op(tokens);
        
        if (op == OP_DEG) {
            node_r = DEG(node_r, node_l);
        }
    }

    return node_r;
}

Node *get_unary_op(Tokenizer *tokens) {

    Node *node_r = get_bracket(tokens);

    if (TOKEN_TYPE(TP_OPERATION) && (TOKEN_OP(OP_SIN) || TOKEN_OP(OP_COS) || TOKEN_OP(OP_LN))) {

        char op = *tokens->array[tokens->size].elem;
        tokens->size++;        

        Node *node_l = get_bracket(tokens);
        
        if (op == OP_COS) {
            node_r = COS(node_l);

        } else if (op == OP_SIN) {
            node_r = SIN(node_l);

        } else if (op == OP_LN) {
            node_r = LN(node_l);
        }

    }

    assert(node_r != NULL);

    return node_r;
}

Node *get_bracket(Tokenizer *tokens) {

    Node *node = NULL;

    if (TOKEN_TYPE(TP_BRACKET)) {

        if (*tokens->array[tokens->size].elem == '(') {
            tokens->size++;        

            node = get_add_sub(tokens);

            assert(*tokens->array[tokens->size].elem == ')');

            tokens->size++;
        }

    } else if (TOKEN_TYPE(TP_NUMBER)) {

        node = get_number(tokens);

    } else if (TOKEN_TYPE(TP_VAR)) {

        node = get_var(tokens);

    }

    return node;
}

Node *get_number(Tokenizer *tokens) {

    Node *node = CREATE_NUM(atoi(tokens->array[tokens->size].elem));

    tokens->size++;  

    assert(node != NULL);

    return node;
}

Node *get_var(Tokenizer *tokens) {
    Node *node = NULL;

    node = CREATE_VAR(tokens->array[tokens->size].elem);

    tokens->size++;        

    assert(node != NULL);

    return node;

}
// -------------------------------------END RECURSIVE DESCENT---------------------------------------------------


// -------------------------------------BEGIN TOKENIZER---------------------------------------------------

int is_oper(char symbol) {

    return (symbol == OP_ADD || symbol == OP_SUB || symbol == OP_DIV || symbol == OP_DEG
         || symbol == OP_SIN || symbol == OP_COS || symbol == OP_LN  || symbol == OP_MUL);

}

void create_number_token(Tokenizer *tokens, char **text_buf, int ip) {

    tokens->array[ip].type = TP_NUMBER;
    char *point = tokens->array[ip].elem;

    while ('0' <= **text_buf && **text_buf <= '9') {
        *point++ = **text_buf;
        (*text_buf)++;
    }

    (*text_buf)--;
}

void create_oper_token(Tokenizer *tokens, char **text_buf, int ip) {

    tokens->array[ip].type = TP_OPERATION;
    char *point = tokens->array[ip].elem;
    
    if (**text_buf == OP_SIN || **text_buf == OP_COS) {

        *point++ = **text_buf;
        (*text_buf)++;
        *point++ = **text_buf;
        (*text_buf)++;
        *point++ = **text_buf;

    } else if (**text_buf == OP_LN) {

        *point++ = **text_buf;
        (*text_buf)++;
        *point++ = **text_buf;

    } else {
        *point++ = **text_buf; 
    }  

}

void create_var_token(Tokenizer *tokens, char **text_buf, int ip) {

    tokens->array[ip].type = TP_VAR;
    char *point = tokens->array[ip].elem;

    while (isalpha(**text_buf)) {
        *point++ = **text_buf;
        (*text_buf)++;
    }

    (*text_buf)--;
}

void tokenizer_ctor(Tokenizer *tokens, char *text_buf) {

    assert(tokens   != NULL && "null poiner tokens");
    assert(text_buf != NULL && "null poiner text_buf");

    int size_buf = strlen(text_buf);
    const char max_size_oper = 3;

    tokens->array = (token_elem *) calloc(size_buf, sizeof(token_elem));

    int i = 0;

    for (i = 0; i < size_buf && *text_buf != '\0'; i++, text_buf++) {

        if ('0' <= *text_buf && *text_buf <= '9') {
            
            tokens->array[i].elem = (char *) calloc(size_buf, sizeof(char));
            create_number_token(tokens, &text_buf, i);
            
        } else if (is_oper(*text_buf)) {
            
            tokens->array[i].elem = (char *) calloc(max_size_oper + 1, sizeof(char));
            create_oper_token(tokens, &text_buf, i);
        

        } else if (isalpha(*text_buf)) {
            tokens->array[i].elem = (char *) calloc(size_buf, sizeof(char));
            create_var_token(tokens, &text_buf, i);
            
        } else if (*text_buf == ')' || *text_buf == '(') {
            tokens->array[i].type = TP_BRACKET;
            tokens->array[i].elem = (char *) calloc(1, sizeof(char));
            *tokens->array[i].elem = *text_buf;
        }
    }
    tokens->capacity = i;
    tokens->size = 0;
}

void tokenizer_dtor(Tokenizer *tokens) {

    for (int i = 0; i < tokens->capacity; i++) {
        free(tokens->array[i].elem);
    }

    free(tokens->array);
}

// -------------------------------------END TOKENIZER---------------------------------------------------



//--------------------------------------BEGIN TREE OUNPUT--------------------------------------------------------

bool checking_for_priority(Node *node, Node *parent) {
    if (!node || !parent) return false;

    if (parent->type == TP_OPERATION && node->type == TP_OPERATION && 
        ((parent->value.oper == OP_ADD || parent->value.oper == OP_SUB) || (node->value.oper == parent->value.oper)))
        return true;

        // исправить тут черезе return выражения

    else return false;
}

void print_tree(Node *node, Node *parent) {

    if (!node) return;
// вывод скобок кринж
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

        } else if (!(!node->left && node->right) && parent) {
            fprintf(file_tree, ")");
        }
        
    }
    
}


void print_node(FILE *file, Node *node) {
    assert(node && "node null");

    switch (node->type) {
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
                    fprintf(file, "didn't find oper");
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

//----------------------------------------BEGIN TEX_DUMP-----------------------------------------------------------

void title_tex_dump(Node *node) {

    const char *FILE_TEX = "res/tex_dump1.tex";
    // const char *FILE_TEX = "../res/tex_dump1.tex";

    FILE *file_tex = fopen(FILE_TEX, "w");

    fprintf(file_tex, "\\documentclass[12pt, letterpaper, twoside]{article}\n");
    fprintf(file_tex, "\\usepackage[T1]{fontenc}\n");
    fprintf(file_tex, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file_tex, "\\usepackage[russian]{babel}\n");

    fprintf(file_tex, "\\title{ВЗЯТИЕ ПРОИЗВОДНОЙ КРОКОДИЛА}\n");
    fprintf(file_tex, "\\author{Очевидно, что Кузнецова Елизавета Юрьевна}\n");
    fprintf(file_tex, "\\date{22.11.2022}\n");
    

    fprintf(file_tex, "\\begin{document}\n");
    fprintf(file_tex, "\\maketitle\n");

    fprintf(file_tex, "\\newpage\n");
    dump_tree(node);
    fprintf(file_tex, "Подробное описание взятия производной заданной функции:\n");
    fprintf(file_tex, "\\[f(x) =");
    print_tex(file_tex, node, NULL);
    fprintf(file_tex, "\\]\n\n");


    node = diff_tree(node);
    fprintf(file_tex, "Производная заданной функции:\n");
    fprintf(file_tex, "\\[f'(x) =");
    print_tex(file_tex, node, NULL);
    fprintf(file_tex, "\\]\n\n");


    optimizer_tree(node);
    fprintf(file_tex, "Упростим данного крокодила(что является достаточно очевидной задачей):\n");
    fprintf(file_tex, "\\[f'(x) =");
    print_tex(file_tex, node, NULL);
    fprintf(file_tex, "\\]\n\n");
    fprintf(file_tex, "\\end{document}\n");

    print_tree(node, NULL);
    
    fclose(file_tex);

    system("pdflatex res/tex_dump1.tex >res/text_tex.txt");
}

void print_tex(FILE *file_tex, Node *node, Node *parent) {

    if (!node) return;

    if (checking_for_priority(node, parent))
        ;
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

    switch (node->type) {
        case TP_OPERATION:
            switch (node->value.oper) {
                case OP_SIN:
                    fprintf(file_tex, "sin");
                    break;

                case OP_COS:
                    fprintf(file_tex, "cos");
                    break;

                case OP_LN:
                    fprintf(file_tex, "ln");
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
    copy_node->type   = node->type;

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

    node->type = tp_node;
    node->left = node_left;
    node->right = node_right;

    switch (tp_node) {
        case TP_NUMBER: {
            node->value.number = value;
            break;
        }

        case TP_OPERATION: {
            node->value.oper = (TYPE_OPERATION)value;
            break;
        }

        case TP_VAR: {
            // char *var = (char *)calloc(MAX_SIZE, sizeof(char));
            // *var = (char)value;
            // node->value.var = var;
            // break;
        }

        default:
            printf("didn't find type node\n");
            break;
    }
    // printf("%d ", value);

    return node;
}

Node *create_var_node(char *var, Node *node_left, Node *node_right) {

    Node *node = (Node *)calloc(1, sizeof(Node));

    node->type = TP_VAR;
    node->left = node_left;
    node->right = node_right;

    node->value.var = (char *) calloc(strlen(var), sizeof(char));
    node->value.var = strcpy(node->value.var, var);

    return node;
}


Node *diff_tree(Node *node) {
   
    switch (node->type) {
        case TP_NUMBER: return CREATE_NUM(0);

        case TP_VAR:    return CREATE_NUM(1);

        case TP_OPERATION: {
            switch (node->value.oper) {
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
                    if (node->left->type == TP_NUMBER) {
                        return MUL(DEG(cL, cR), MUL(dR, LN(cL)));
                    } else {
                        return MUL(MUL(cR, DEG(cL, SUB(cR, CREATE_NUM(1)))), dL);
                    }

                case OP_LN:
                    if (node->right->type == TP_NUMBER) {
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

//-----------------------------------------END DIFFERENTIATOR-----------------------------------------------------------



//-----------------------------------------BEGIN OPTIMIZER--------------------------------------------------------------
 
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
            node->type = TP_NUMBER;
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

        } else if (node->type == TP_OPERATION && IS_CONST_NODE(node)) {
            OP_CONST(node->value.oper);
            *continue_optimiz = 1;
        }
    }

    folding_constant(node->right, continue_optimiz);

    return *continue_optimiz;
}

int pow(int x, int y) {
    while (y > 1) {
        x *= x;
        y--;
    }
    return x;
}
//-------------------------------END OPTIMIZER--------------------------------------------
