/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** MenuGame
*/

#ifndef INCLUDED_MENUGAME_HPP
    #define INCLUDED_MENUGAME_HPP

#include "AGameModule.hpp"
#include "IGameModule.hpp"
#include <iostream>
#include <vector>

class MenuGame : public AGameModule
{
    private:
        const std::string _name = "MenuGame";
        IDisplayModule* _display; //we will store the display module here so we can use it in the game logic
        int _width = 10; //will be dynamic in the future
        int _height = 10; //will be dynamic in the future

        std::vector<std::string> _gameLibs; //we will store the names of the game libraries here
        std::vector<std::string> _graphicalLibs; //we will store the names of the graphical libraries here

        void loadLibs(); //this will load the names of the libs in the lib folder into the _gameLibs and _graphicalLibs vectors

        bool selected_option = false; //this will be set to true when the user selects either a game or a graphical library, and it will be used to determine when to exit the menu and start the game
        int current_game_index = 0; //this will keep track of the currently selected game index in the _gameLibs vector
        int current_graphical_index = 0; //this will keep track of the currently selected graphical index in the _graphicalLibs vector
        int global_index = 0; //this will keep track of the currently selected index in the overall menu
    public:
        MenuGame();
        ~MenuGame() = default;
        std::tuple<std::string, std::string> get_path_chosen(); //game lib path, graph lib path

        const std::string &getName() const override;
        void load_display(IDisplayModule* display) override;
        void tick(EventType input) override;
        void exit() override;

};

#endif
