#ifndef CALCULATOR_HPP_INCLUDED
#define CALCULATOR_HPP_INCLUDED

#include <stdio.h>
#include <assert.h>
#include "../include/DSL.hpp"

Node *get_g(char **text_prog);
Node *get_e(char **text_prog);
Node *get_t(char **text_prog);
Node *get_d(char **text_prog);
Node *get_u(char **text_prog);
Node *get_p(char **text_prog);
Node *get_number(char **text_prog);
Node *get_var(char **text_prog);

#endif