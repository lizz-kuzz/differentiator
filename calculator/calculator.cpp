#include "../include/diff_function.hpp"
#include "../include/DSL.hpp"
#include <assert.h>

Node *get_g(char **text_prog) {
    Node *val = get_e(text_prog);

    assert(**text_prog == '\0');

    return val;
}

Node *get_e(char **text_prog) {
    Node *val = get_t(text_prog);

    while (**text_prog == OP_ADD || **text_prog == OP_SUB) {

        char op = **text_prog;
        (*text_prog)++;

        Node *val2 = get_t(text_prog);

        if (op == OP_ADD) {
            val = ADD(val, val2);
        } else if (op == OP_SUB) {
            val = SUB(val, val2);
        }
    }

    return val; 
}


Node *get_t(char **text_prog) {
    Node *val = get_d(text_prog);
    

    while (**text_prog == OP_SUB || **text_prog == OP_DIV) {

        char op = **text_prog;
        (*text_prog)++;

        Node *val2 = get_d(text_prog);
        
        if (op == OP_MUL) {
            val = MUL(val, val2);
        } else if (op == OP_DIV) {
            val = DIV(val, val2);
        }
    }
    return val;
}

Node *get_d(char **text_prog) {

    Node *val = get_u(text_prog);
    

    while (**text_prog == OP_DEG) {

        char op = **text_prog;
        (*text_prog)++;

        Node *val2 = get_u(text_prog);
        
        if (op == OP_DEG) {
            val = DEG(val, val2);
        }
    }

    return val;
}

Node *get_u(char **text_prog) {

    Node *val = get_p(text_prog);
    
    char op = **text_prog;

    if (op == OP_COS || op == OP_SIN || op == OP_LN) {

        if (op == OP_COS || op == OP_SIN) {
            printf("%c\n", **text_prog);
            (*text_prog) += 3;
        } else {
            (*text_prog) += 2;
        } 

        Node *val2 = get_p(text_prog);

        // assert(**text_prog == '\0');
        
        // (*text_prog)++;
        
        
        if (op == OP_COS) {
            val = COS(val2);
        } else if (op == OP_SIN) {
            val = SIN(val2);
        } else if (op == OP_LN) {
            val = LN(val2);
        }
    }
    assert(val != NULL);
    return val;
}

Node *get_p(char **text_prog) {

    Node *node = NULL;

    if (**text_prog == '(') {
        (*text_prog)++;

        node = get_e(text_prog);

        assert(**text_prog == ')');

        (*text_prog)++;
        
    } else if ('0' <= **text_prog && **text_prog <= '9') {
        node = get_number(text_prog);
    } else if (**text_prog == 'x') {
        node = get_var(text_prog);
    }


    return node;
}

Node *get_number(char **text_prog) {

    int val = 0; 
    Node *node = {};
    
    const char *old_text_prog = *text_prog;

    if ('0' <= **text_prog && **text_prog <= '9') {

        while ('0' <= **text_prog && **text_prog <= '9') {
            val += **text_prog - '0';
            (*text_prog)++;
        }
        
        node = CREATE_NUM(val);
    } 

    assert(*text_prog != old_text_prog);
    return node;
}

Node *get_var(char **text_prog) {
    Node *node = NULL;

    if (isalpha(**text_prog)) {
        node = CREATE_VAR(**text_prog);
        (*text_prog)++;
    } 

    assert(node != NULL);

    return node;

}