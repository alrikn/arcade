/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SnakeGame
*/

#include "SnakeGame.hpp"
#include "IGameModule.hpp"
#include <algorithm>

//for now we'll assume that the width and height of the game is 10*10 but in the future display and width will need to be given by the display module, and the game will need to adapt to it, but for now we'll just hardcode it for testing purposes.
SnakeGame::SnakeGame()
{
    std::cout << "[" << _name << "] Constructor called" << std::endl;


}

const std::string &SnakeGame::getName() const
{
    return _name;
}

void SnakeGame::load_display(IDisplayModule* display)
{
    std::cout << "[" << _name << "] load_display called with display: " << display->getName() << std::endl;
    _display = display; //we store the display module so we can use it in the game logic

    //we will fill game map with empty tiles for now, but in the future we will need to add the snake and the food to the game map, and we will need to update the game map every tick based on the game logic
    _gameMap = std::vector<std::vector<ShapeType>>(_height, std::vector<ShapeType>(_width, EMPTY));

    //now we load the player on 1,1 for testing purposes, but in the future we will need to generate the player position randomly, and we will need to update the player position every tick based on the game logic
    _gameMap[player_x][player_y] = SQUARE;
}

//for now we'll assume that the width and height of the game is 10*10 but in the future display and width will need to be given by the display module, and the game will need to adapt to it, but for now we'll just hardcode it for testing purposes.
void SnakeGame::tick()
{
    _display->pollEvents(); //we poll for events every tick
    auto events = _display->getEvents();//user input

    //now we check for if for directions.
    //very ugly, we'll cahnge later
    if (std::find(events.begin(), events.end(), W_KEY) != events.end()) {
        player_y--;
    }
    if (std::find(events.begin(), events.end(), A_KEY) != events.end()) {
        player_x--;
    }
    if (std::find(events.begin(), events.end(), S_KEY) != events.end()) {
        player_y++;
    }
    if (std::find(events.begin(), events.end(), D_KEY) != events.end()) {
        player_x++;
    }

    _gameMap[player_x][player_y] = SQUARE;

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            _display->drawTile(_gameMap[y][x], 0, x, y); //we'll just use color 0 for now, but in the future we will need to use different colors for different tiles, and we will need to update the colors based on the game logic
        }
    }
}

void SnakeGame::loop()
{
    std::cout << "[" << _name << "] loop called" << std::endl;
}

void SnakeGame::exit()
{
    std::cout << "[" << _name << "] exit called" << std::endl;
}

//C interface (THIS is what dlopen/dlsym uses)
extern "C" {

IGameModule* create()
{
    return new SnakeGame();
}

void destroy(IGameModule* instance)
{
    delete instance;
}

}