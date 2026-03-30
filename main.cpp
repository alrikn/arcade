/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** main
** this will call the shared objects.
** a loop to load each library, call the myEntryPoint functions, and close each library
*/

#include "Core.hpp"



int main(int argc, char **argv)
{
    if (argc > 1) {
        Core core((std::string(argv[1])));
        core.run();
    } else {
        Core core;
        core.run();
    }
    return 0;
}
