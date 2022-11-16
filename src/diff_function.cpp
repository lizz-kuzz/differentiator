#include "../include/diff_function.hpp"
#include "../include/file.hpp"
const char *NAME_FILE = "/mnt/c/Users/User/Desktop/programs/differentiator/res/text_output.txt";
FILE *file_tree = fopen(NAME_FILE, "w");

const int MAX_SIZE = 10;

void create_tree(const char *FILE_INPUT, Tree *tree) {
    char *text_buf;

    text_buf = read_file(FILE_INPUT, text_buf);
    char *point_text_buf = text_buf;
    tree->root_tree = (Node *) calloc(1, sizeof(Node));
    // point_text_buf++;
    // tree->root_tree =
     create_tree_from_text(tree->root_tree, &point_text_buf);

    free(text_buf);
}

Node *create_tree_from_text(Node *node, char **text_buf) {
    if (**text_buf == '\0') return node;
    printf("buf %s\n", *text_buf);

    if (**text_buf == '(') {
        node->left = (Node *) calloc(1, sizeof(Node));
        printf("левый\n");
        (*text_buf)++;
        node->left = create_tree_from_text(node->left, text_buf);
    }

    char *elem = (char *) calloc(MAX_SIZE, sizeof(char));
    char *point = elem;
    printf("buf %s\n", *text_buf);
    for (; **text_buf != ')' && **text_buf != '(' && **text_buf != '\0' && **text_buf != '\n'; (*text_buf)++, elem++) *elem = **text_buf; 
    elem = point;

    printf("buf %s\n", *text_buf);

    printf("create %s\n", elem);

    node = tree_add_elem(node, elem);


    if (**text_buf == '(') { 
        (*text_buf)++;
        node->right = (Node *) calloc(1, sizeof(Node));
        printf("правый\n");
        node->right = create_tree_from_text(node->right, text_buf);
    }

    if (**text_buf == ')') {
        (*text_buf)++; 
        printf("вернулись\n");
        return node;
    }

    printf("конец %s\n", node->var_value);
    return node;
}

Node *tree_add_elem(Node *node, tree_elem_t elem) {

    // node = (Node *) calloc(1, sizeof(Node));
    // printf("add %s\n", elem);
    node->var_value = (tree_elem_t) calloc(MAX_SIZE, sizeof(char));
    node->var_value = strcpy((char *)node->var_value, elem);
    // printf("node %s\n",node->var_value );
    

    // передовать в эту функцию чаровское число, а тут делать разбор случев

    // node->left = NULL;
    // node->right = NULL;

    return node;
}


void printf_tree(Node *node) {

    if (!node) return;

    if (!node->left && !node->right) {

        fprintf(file_tree, "(%s)", node->var_value);

    } else {

        fprintf(file_tree, "(");
        fprintf(file_tree, "%s", node->var_value);

        if (node->left) {
            printf_tree(node->left);
        }

        if (node->right) {
            printf_tree(node->right);
            fprintf(file_tree, ")");
        }
    }
}

void close_file() {
    fclose(file_tree);
}
