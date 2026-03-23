/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SnakeGame
*/

#ifndef INCLUDED_SNAKEGAME_HPP
    #define INCLUDED_SNAKEGAME_HPP

#include <iostream>
#include <chrono>
#include "AGameModule.hpp"
#include "IDisplayModule.hpp"
#include "IGameModule.hpp"
#include <deque>
#include <vector>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class SnakeGame : public AGameModule
{
    private:
        const std::string _name = "SnakeGame";

        Direction _currentDir = RIGHT;
        Direction _nextDir = RIGHT;

        //deque comes from the library <
        std::deque<std::pair<int, int>> _snake; //front = head
        std::pair<int, int> _foodPos;
        unsigned int _score = 0;

        // we need to make a game map for the snake game, we can use a 2D vector of ShapeType for this, and we can use the drawTile function to draw the map based on the shape type
        std::vector<std::vector<ShapeType>> _gameMap;
    public:
        SnakeGame();
        ~SnakeGame() = default;

        const std::string &getName() const override;
        //void load_display(IDisplayModule* display) override;
        void tick(EventType input) override;
        void exit() override;

        void generateFood(); //random food generation

        bool checkCollision(int x, int y); //check for collision with walls and snake body

        void reset_game(EventType input); //display score and high score, until the user presses space to reset game

};


#endif
