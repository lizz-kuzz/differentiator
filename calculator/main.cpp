// #include "calculator.hpp"
#include "../include/file.hpp"
#include "../include/diff_function.hpp"

int main() {
    const char *FILE_PROG = "calculator/test_prog.txt";
    // system("pwd");
    char *text_prog = NULL;
    text_prog = read_file(FILE_PROG, text_prog);
    printf("%s\n", text_prog);

    Node *node = get_g(&text_prog);
    dump_tree(node);
    title_tex_dump(node);
    return 0;
}