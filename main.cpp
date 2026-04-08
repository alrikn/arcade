/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** main
** this will call the shared objects.
** a loop to load each library, call the myEntryPoint functions, and close each library
*/

#include "Core.hpp"
#include <cstdlib>
#include <exception>

bool file_exists(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}

//checcks the the lib name that we have been given, and checks if its one in lib/graphical_lib,
//if it is, it returns the path to the library
std::string get_graphical_path(std::string graph_path_user)
{
    if (!file_exists(graph_path_user)) {
        throw CoreError("Graphical library not found: " + graph_path_user);
    }
    //now we check if the same file exists in the graphical lib folder, if it does, we use that one instead

    //example if they give us ../random_dir/arcade_ncurses.so, we check if ./lib/graphical_lib/arcade_ncurses.so exists, and if it does, we use that one instead
    std::string lib_name = graph_path_user.substr(graph_path_user.find_last_of("/\\") + 1);
    std::string lib_path = "./lib/graphical_lib/" + lib_name;
    std::cout<< "Checking for graphical library at: " << lib_path << std::endl;
    std::cout << "original user path: " << graph_path_user << std::endl;
    if (file_exists(lib_path)) {
        return lib_path;
    }
    throw CoreError("Graphical library is not recognised: " + graph_path_user);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <graphical_lib_name>" << std::endl;
        std::cerr << "Available graphical libraries: ncurses, sfml, sdl2" << std::endl;
        return 84;
    }
    std::string graph_lib;
    try {
        graph_lib = get_graphical_path(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    Core core(graph_lib);
    try {
        core.run();
    } catch (const std::exception& e) {
        std::cerr << "Error :" << e.what() << std::endl;
        return 84;
    }

    return 0;
}
