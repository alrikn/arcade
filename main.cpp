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
#include "MenuGame.hpp"


int main(void)
{
    DLLoader<IDisplayModule> graphical_loader("./lib/graphical_lib/libNcurses.so");

    IDisplayModule* graphical_module = graphical_loader.getInstance();

    printf("graphicalName: %s\n", graphical_module->getName().c_str());

    DLLoader<IGameModule> game_loader("./lib/game_lib/arcade_snake.so");

    IGameModule* game_module = game_loader.getInstance();
    printf("gameName: %s\n", game_module->getName().c_str());

    game_module->load_display(graphical_module);

    bool menu = false; //will need to be default true after
    while (true) {
        EventType event = graphical_module->pollEvents();

        if (event == QUIT || event == MENU)
            menu = true;
        if (menu) {
            printf("Currently in the menu\n"); //debugging purposes
            //here we load a game that does the meny callings
            //MenuGame menu_game;
            continue;
        }
        game_module->tick(event); //we will need to pass the events that were polled from the display module here, but for now we'll just pass a dummy event for testing purposes
    }

    //graphical_module->display_menu();
    return 0;
}
