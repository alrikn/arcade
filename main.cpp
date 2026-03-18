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

    bool menu = true;
    MenuGame menu_game;
    while (true) {
        EventType event = graphical_module->pollEvents();

        if (event == QUIT || event == MENU)
            menu = true;
        if (menu) {
            menu_game.load_display(graphical_module);
            while (menu) {
                EventType menu_event = graphical_module->pollEvents();
                if (menu_event == QUIT) {
                    menu = false;
                    break;
                }
                menu_game.tick(menu_event);
                auto [gameLibPath, graphLibPath] = menu_game.get_path_chosen();
                if (gameLibPath != "" && graphLibPath != "") {
                    // first unload current game + graphical game if there is one
                    game_module->exit(); //we call exit to let the game do any cleanup it needs to do before we unload the library
                    graphical_module->stop(); //we call stop to let the graphical module do any cleanup it
                    game_loader.reset(); //we unload the current game library
                    graphical_loader.reset(); //we unload the current graphical library

                    // then we load the new game + graphical game
                    graphical_loader.setHandle(graphLibPath);
                    graphical_module = graphical_loader.getInstance();
                    game_loader.setHandle(gameLibPath);
                    game_module = game_loader.getInstance();
                    game_module->load_display(graphical_module);

                    menu = false;
                }
            }
            continue;
        }
        game_module->tick(event); //we will need to pass the events that were polled from the display module here, but for now we'll just pass a dummy event for testing purposes
    }

    //graphical_module->display_menu();
    return 0;
}
