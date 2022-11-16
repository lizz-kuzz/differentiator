#include "../include/diff_function.hpp"
#include "../include/file.hpp"

int main(int argc, char *argv[]) {

    // char *path = "/mnt/c/Users/User/Desktop/programs/differentiator/res/";
    // char *name_file = argv[1];
    // char *FILE_INPUT = (char *) calloc(strlen(path) + strlen(name_file), sizeof(char));
    // create_file_path(FILE_INPUT, path, name_file);
    const char *FILE_INPUT = "/mnt/c/Users/User/Desktop/programs/differentiator/res/text.txt";

    Tree tree = {};

    ctor_tree(FILE_INPUT, &tree);
    
    printf_tree(tree.root_tree);
    dump_tree(tree.root_tree);

    dtor_tree(tree.root_tree);
}