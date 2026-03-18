/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** main
** this will call the shared objects.
** a loop to load each library, call the myEntryPoint functions, and close each library
*/

#include <dlfcn.h>
#include <stdio.h>
#include "IDisplayModule.hpp"
#include "DLLoader.hpp"
#include "IGameModule.hpp"
#include "LibFoo.hpp"


int main(void)
{
    DLLoader<IDisplayModule> graphical_loader("./lib/graphical_lib/libNcurses.so");

    IDisplayModule* graphical_module = graphical_loader.getInstance();

    printf("graphicalName: %s\n", graphical_module->getName().c_str());

    DLLoader<IGameModule> game_loader("./lib/game_lib/arcade_snake.so");

    IGameModule* game_module = game_loader.getInstance();
    printf("gameName: %s\n", game_module->getName().c_str());

    game_module->load_display(graphical_module);

    game_module->loop();

    graphical_module->display_menu();
    return 0;
}
