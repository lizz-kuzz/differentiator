#include "calculator.hpp"

int get_g(char **text_prog) {
    int val = get_e(text_prog);
    assert(**text_prog == '\0');
    return val;
}

int get_e(char **text_prog) {
    int val = get_t(text_prog);
    while (**text_prog == '+' || **text_prog == '-') {
        char op = **text_prog;
        (*text_prog)++;
        int val2 = get_t(text_prog);
        if (op == '+') {
            val += val2;
        } else {
            val -= val2;
        }
    }
    return val; 
}

int get_t(char **text_prog) {
    int val = get_p(text_prog);
    while (**text_prog == '*' || **text_prog == '/') {
        char op = **text_prog;
        (*text_prog)++;
        int val2 = get_p(text_prog);
        if (op == '*') {
            val *= val2;
        } else {
            val /= val2;
        }
    }
    return val;
}

int get_p(char **text_prog) {
    int val = 0;
    if (**text_prog == '(') {
        (*text_prog)++;
        val = get_e((text_prog));
        assert(**text_prog == ')');
        (*text_prog)++;
    } else {
        val = get_n(text_prog);
    }
    return val;
}

int get_n(char **text_prog) {
    int val = 0; 
    const char *old_text_prog = *text_prog;
    if ('0' <= **text_prog && **text_prog <= '9') {
        val = **text_prog - '0';
        (*text_prog)++;
    }
    assert(*text_prog != old_text_prog);
    return val;
}