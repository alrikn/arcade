/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Core
*/

#include "Core.hpp"
#include <cstdio>

Core::Core()
{
    printf("do smth");
}

void Core::run()
{
    graphical_module = graphical_loader.getInstance();
    game_module = game_loader.getInstance();

    printf("graphicalName: %s\n", graphical_module->getName().c_str());
    printf("gameName: %s\n", game_module->getName().c_str());

    game_module->load_display(graphical_module);

    while (true) {
        EventType event = graphical_module->pollEvents();

        if (event == QUIT || event == MENU)
            _menu = true;
        if (_menu) {
            menu_handle();
        }
        else {
            game_module->tick(event);
            graphical_module->draw();
        }
    }
}

void Core::menu_handle()
{
    _menu_game.load_display(graphical_module);
    while (_menu) {
        EventType menu_event = graphical_module->pollEvents();
        //if (menu_event == QUIT) {
        //    _menu = false;
        //    break;
        //}
        _menu_game.tick(menu_event);
        auto [gameLibPath, graphLibPath] = _menu_game.get_path_chosen();
        if (gameLibPath != "" && graphLibPath != "") {
            // first unload current game + graphical game if there is one
            game_module->exit(); //we call exit to let the game do any cleanup it needs to do before we unload the library
            graphical_module->stop(); //we call stop to let the graphical module do any cleanup it needs to do before we unload the library
            game_loader.reset(); //we unload the current game library
            graphical_loader.reset(); //we unload the current graphical library

            // then we load the new game + graphical game
            graphical_loader.setHandle(graphLibPath);
            graphical_module = graphical_loader.getInstance();

            game_loader.setHandle(gameLibPath);
            game_module = game_loader.getInstance();

            game_module->load_display(graphical_module);

            _menu = false; //we exit the menu and start the game
        }
    }
}
