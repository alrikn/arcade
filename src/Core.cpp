/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Core
*/

#include "Core.hpp"
#include "IDisplayModule.hpp"
#include <cstdio>

Core::Core(std::string graphical_lib) :
    graphical_loader(graphical_lib),
    game_loader(_currentGameLib),
    _menu_game(_currentGameLib, graphical_lib)
{
    _currentGraphicalLib = graphical_lib;
}

void Core::update_event()
{
    EventType event = graphical_module->pollEvents();

    if (event != OTHER)
        _lastEvent = event;
}

void Core::go_next_lib(EventType event)
{
    if (event == NUM_1 || event == NUM_2) {
        bool previous = (event == NUM_1);
        std::string nextGameLib = _menu_game.get_next_game(previous);
        load_new_game(nextGameLib);
    }
    if (event == NUM_3 || event == NUM_4) {
        bool previous = (event == NUM_3);

        std::string nextGraphLib = _menu_game.get_next_graphical(previous);
        load_new_graphical(nextGraphLib);
    }
    _lastEvent = OTHER; //we reset the last event to other so we don't immediately repeat the same event again
}

void Core::run()
{
    graphical_module = graphical_loader.getInstance();
    game_module = game_loader.getInstance();

    printf("graphicalName: %s\n", graphical_module->getName().c_str()); //crashes here (this is line 38)
    printf("gameName: %s\n", game_module->getName().c_str());

    //mem leak comes from here.
    game_module->load_display(graphical_module);
    _menu_game.load_display(graphical_module);
    _elapsed = _menu_game.get_elapsed();
    graphical_module->init();

    while (_running) {
        update_event();

        if ((_lastEvent == QUIT || _lastEvent == MENU) && !_menu) {
            _menu = true;
            _menu_game.update_highscore(game_module->getName(), game_module->get_highscore());
            _elapsed = _menu_game.get_elapsed();
            _lastEvent = OTHER; //we reset the last event to other so we don't immediately exit the menu again
        }
        if (_lastEvent == NUM_1 || _lastEvent == NUM_2 || _lastEvent == NUM_3 || _lastEvent == NUM_4) {
            go_next_lib(_lastEvent);
            continue;
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
        graphical_module->draw();
    }
}

void Core::menu_handle()
{
    update_event();
    if (_lastEvent == QUIT) {
        _menu = false;
        _running = false;
        return;
    }
    _menu_game.tick(_lastEvent);
    auto [gameLibPath, graphLibPath] = _menu_game.get_path_chosen();
    if (gameLibPath != "" && graphLibPath != "") {
        _menu = false; //the default is to set the menu to false, but we'll set it back to true if what the user has selected is a graphical lib
        if (gameLibPath != _currentGameLib)
            load_new_game(gameLibPath);
        if (graphLibPath != _currentGraphicalLib) {
            load_new_graphical(graphLibPath);
            _menu = true; //if user has selected new graph, doesn't mean that he wants to play, just that hes sbrowsing what things look like
        }
        _elapsed = game_module->get_elapsed(); //we update the elapsed time based on the new game's settings
    }
    if (player_name.empty())
        player_name = _menu_game.get_player_name();
}

void Core::load_new_game(std::string game_path)
{
    //before anything we update the high score
    _menu_game.update_highscore(game_module->getName(), game_module->get_highscore());

    //now thats done we can exit
    game_module->exit();
    game_loader.reset();

    game_loader.setHandle(game_path);
    game_module = game_loader.getInstance();
    game_module->load_display(graphical_module);

    _currentGameLib = game_path;
}

void Core::load_new_graphical(std::string graphical_path)
{
    std::cout << "old path: " << _currentGraphicalLib << std::endl;
    std::cout << "new path: " << graphical_path << std::endl;

    graphical_module->stop();
    graphical_loader.reset();

    graphical_loader.setHandle(graphical_path);
    graphical_module = graphical_loader.getInstance();
    game_module->load_display(graphical_module);
    _menu_game.load_display(graphical_module); //we also need to reload the menu with the new graphical library, otherwise when we go back to the menu it will use the old graphical library which is now unloaded, and that will cause a crash when we try to draw with it.

    _currentGraphicalLib = graphical_path;
    graphical_module->init();
}
