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

            if (gameLibPath != _currentGameLib)
                load_new_game(gameLibPath);
            if (graphLibPath != _currentGraphicalLib)
                load_new_graphical(graphLibPath);

            _menu = false; //we exit the menu and start the game
        }
    }
}

void Core::load_new_game(std::string game_path)
{
    game_module->exit();
    game_loader.reset();

    game_loader.setHandle(game_path);
    game_module = game_loader.getInstance();
    game_module->load_display(graphical_module);

    _currentGameLib = game_path;
}

void Core::load_new_graphical(std::string graphical_path)
{
    graphical_module->stop();
    graphical_loader.reset();

    graphical_loader.setHandle(graphical_path);
    graphical_module = graphical_loader.getInstance();
    game_module->load_display(graphical_module);

    _currentGraphicalLib = graphical_path;
}
