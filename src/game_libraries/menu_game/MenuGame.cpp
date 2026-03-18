/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** MenuGame
*/

#include "MenuGame.hpp"
#include "IDisplayModule.hpp"
#include<dirent.h>

MenuGame::MenuGame()
{
    std::cout << "[" << _name << "] Constructor called" << std::endl;
    loadLibs();
}

const std::string &MenuGame::getName() const
{
    return _name;
}

void MenuGame::loadLibs()
{
    //we loop through all files in lib/game_lib and put them in _gameLibs
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./lib/game_lib")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;
            if (fileName.size() > 3 && fileName.substr(fileName.size() - 3) == ".so") {
                _gameLibs.push_back(fileName);
            }
        }
        closedir(dir);
    }

    //we loop through all files in lib/graphical_lib and put them in _graphicalLibs
    if ((dir = opendir("./lib/graphical_lib")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;
            if (fileName.size() > 3 && fileName.substr(fileName.size() - 3) == ".so") {
                _graphicalLibs.push_back(fileName);
            }
        }
        closedir(dir);
    }
}

void MenuGame::load_display(IDisplayModule* display)
{
    std::cout << "[" << _name << "] load_display called with display: " << display->getName() << std::endl;
    _display = display; //we store the display module so we can use it in the game logic
    _display->init();

    _height = _display->getHeight();
    _width = _display->getWidth();

    //we should also check all of the .so files in the lib folder to be able to display them
}

void MenuGame::tick(EventType input)
{
    //input handle
    int maxIndex = _gameLibs.size() + _graphicalLibs.size() - 1;

    if (input == W_KEY && global_index > 0)
        global_index--;
    if (input == S_KEY && global_index < maxIndex)
        global_index++;
    if (input == SPACE_KEY)
        selected_option = true;

    //global index handle
    if (global_index < (int)_gameLibs.size()) {
        current_game_index = global_index;
    } else {
        current_graphical_index = global_index - _gameLibs.size();
    }

    //render hander
    _display->clear();

    int y = 0;
    int index = 0;

    _display->drawText("Welcome to the Arcade!", 0, y++);
    _display->drawText("Use W/S to navigate", 0, y++);
    _display->drawText("Press SPACE to select", 0, y++);
    _display->drawText("Press Q to quit", 0, y++);
    y++; //for readability

    _display->drawText("Available Game Libraries:", 0, y++);
    for (const auto& lib : _gameLibs) {
        std::string prefix = (global_index == index) ? "> " : "  ";
        _display->drawText(prefix + lib, 2, y++);
        index++;
    }

    y++;

    _display->drawText("Available Graphical Libraries:", 0, y++);
    for (const auto& lib : _graphicalLibs) {
        std::string prefix = (global_index == index) ? "> " : "  ";
        _display->drawText(prefix + lib, 2, y++);
        index++;
    }
}

std::tuple<std::string, std::string> MenuGame::get_path_chosen()
{
    if (selected_option) {
        std::string gameLibPath = "./lib/game_lib/" + _gameLibs[current_game_index];
        std::string graphLibPath = "./lib/graphical_lib/" + _graphicalLibs[current_graphical_index];
        selected_option = false; //reset for next time we go to the menu
        return {gameLibPath, graphLibPath};
    }
    return {"", ""}; //to signify that user has not selected shit
}

void MenuGame::exit()
{
    std::cout << "[" << _name << "] exit called" << std::endl;
}


extern "C" {

IGameModule* create()
{
    return new MenuGame();
}

void destroy(IGameModule* instance)
{
    delete instance;
}

}
