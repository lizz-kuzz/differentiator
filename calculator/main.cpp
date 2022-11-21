#include "calculator.hpp"
#include "../include/file.hpp"

int main() {
    const char *FILE_PROG = "/mnt/c/Users/User/Desktop/programs/differentiator/calculator/test_prog.txt";
    char *text_prog = NULL;
    text_prog = read_file(FILE_PROG, text_prog);
    printf("%s\n", text_prog);

    printf("%d\n", get_g(&text_prog));
}