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

        std::string player_name = ""; //we'll use it at the start of the menu, then we'll pass it to core
        bool name_entered = false; //this will be set to true when the user has entered their name, and it will be used to determine when to show the rest of the menu options
    public:
        MenuGame();
        ~MenuGame() = default;
        std::tuple<std::string, std::string> get_path_chosen(); //game lib path, graph lib path

        const std::string &getName() const override;
        void load_display(IDisplayModule* display) override;
        void tick(EventType input) override;
        void exit() override;
        bool is_name_entered() const { return name_entered; }
        std::string get_player_name() const { return player_name; }

        //is a loop of its own that handles name input
        //gets called only once when the core boots up the menu, and it will keep looping until the user has entered their name, and then it will set name_entered to true and return the name, which will be stored in player_name
        void handle_name_input(void);

};

#endif
