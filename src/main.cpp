#include "../include/diff_function.hpp"
#include "../include/file.hpp"

int main(int argc, char *argv[]) {

    system("rm tex_dump1.pdf");
    system("rm *.aux");
    system("rm *.log");
    char *path = "res/";
    char *name_file = argv[1];
    char *FILE_INPUT = (char *) calloc(strlen(path) + strlen(name_file), sizeof(char));
    create_file_path(FILE_INPUT, path, name_file);
    // const char *FILE_INPUT = "res/text.txt";

    Tree tree = {};

    ctor_tree(FILE_INPUT, &tree);
    dump_tree(tree.root_tree);
    title_tex_dump(tree.root_tree);

    
    tree.root_tree = diff_tree(tree.root_tree);
    diff_func_title_tex_dump(tree.root_tree);  
    dump_tree(tree.root_tree);


    optimizer_tree(tree.root_tree);
    optimiz_tex_dump(tree.root_tree);
    dump_tree(tree.root_tree);
    end_tex_dump();
    
    print_tree(tree.root_tree, NULL);
    dtor_tree(tree.root_tree);
    close_file();
}