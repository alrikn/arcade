/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Core
*/

#ifndef INCLUDED_CORE_HPP
    #define INCLUDED_CORE_HPP


#include "DLLoader.hpp"
#include "IDisplayModule.hpp"
#include "IGameModule.hpp"
#include "MenuGame.hpp"
#include <string>
#include <chrono>
class Core
{
    private:
        const std::string _name = "Core";
        std::string _currentGameLib = "./lib/game_lib/arcade_snake.so";
        std::string _currentGraphicalLib = "./lib/graphical_lib/libNcurses.so";
        bool _menu = true; //we start in the menu, so we set this to true

        DLLoader<IDisplayModule> graphical_loader = DLLoader<IDisplayModule>(_currentGraphicalLib);
        DLLoader<IGameModule> game_loader = DLLoader<IGameModule>(_currentGameLib);

        IDisplayModule* graphical_module;
        IGameModule* game_module;
        MenuGame _menu_game;

        unsigned long _elapsed = 1; // time in millisecond between ticks.

        std::chrono::steady_clock::time_point _lastMoveTime;
        EventType _lastEvent; //last event that isn't other
    public:
        bool _running = true;
        Core();
        ~Core() = default;

        void run(); //this will be the main loop of the program, it will handle loading the libraries, calling the game and display functions, and handling the menu logic

        void menu_handle(); //menu logic

        void load_new_game(std::string game_path);

        void load_new_graphical(std::string graphical_path);

};


#endif
