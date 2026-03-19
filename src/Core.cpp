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

void Core::update_event()
{
    EventType event = graphical_module->pollEvents();

    if (event != OTHER)
        _lastEvent = event;
}

void Core::run()
{
    graphical_module = graphical_loader.getInstance();
    game_module = game_loader.getInstance();

    printf("graphicalName: %s\n", graphical_module->getName().c_str());
    printf("gameName: %s\n", game_module->getName().c_str());

    //this is line 24, and every mem leak comes from here.
    game_module->load_display(graphical_module);
    bool loaded_menu = false;

    while (_running) {
        update_event();

        if ((_lastEvent == QUIT || _lastEvent == MENU) && !_menu) {
            _menu = true;
            _lastEvent = OTHER; //we reset the last event to other so we don't immediately exit the menu again
        }
        if (_menu && !loaded_menu) {
            _menu_game.load_display(graphical_module);
            _elapsed = _menu_game.get_elapsed();
            loaded_menu = true;
        }
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(now - _lastMoveTime).count();
        if (elapsed < _elapsed)
            continue; //not enough time
        if (_menu)
            menu_handle();
        else
            game_module->tick(_lastEvent);
        _lastEvent = OTHER; //we reset the last event to other so we don't immediately repeat the same event again
        _lastMoveTime = std::chrono::steady_clock::now();
    }
}

void Core::menu_handle()
{
    update_event();
    if (_lastEvent == QUIT) {
        _menu = false;
        _running = false;
    }
    _menu_game.tick(_lastEvent);
    auto [gameLibPath, graphLibPath] = _menu_game.get_path_chosen();
    if (gameLibPath != "" && graphLibPath != "") {
        if (gameLibPath != _currentGameLib)
            load_new_game(gameLibPath);
        if (graphLibPath != _currentGraphicalLib)
            load_new_graphical(graphLibPath);
        _menu = false; //we exit the menu and start the game
        _elapsed = game_module->get_elapsed(); //we update the elapsed time based on the new game's settings
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
