/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Nibbler
*/

#ifndef INCLUDED_NIBBLER_HPP
    #define INCLUDED_NIBBLER_HPP

#include <deque>
#include <utility>
#include <vector>
#include <string>
#include "AGameModule.hpp"
#include "IDisplayModule.hpp"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum  CellType {
    CELL_EMPTY,
    CELL_WALL,
    CELL_FOOD
};

class Nibbler : public AGameModule
{
    private:
        const std::string _name = "Nibbler";
        Direction _currentDir = RIGHT;
        Direction _nextDir = RIGHT;
        std::deque<std::pair<int, int>> _snake;
        std::vector<std::vector<CellType>> _map;
        int _foodCount = 0;
        bool _levelWon = false;
    protected:
    public:
        Nibbler();
        ~Nibbler() = default;

        const std::string &getName() const override;
        void tick(EventType input) override;
        void exit() override;

        void loadMap(int rotation);
        void generateSnakeStart();
        bool checkCollision(int x, int y);
        void reset_game(EventType input);
        Direction tryAutoTurn(int x, int y, Direction);
};

#endif
