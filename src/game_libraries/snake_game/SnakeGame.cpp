/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SnakeGame
*/

#include "SnakeGame.hpp"
#include "IDisplayModule.hpp"
#include "IGameModule.hpp"
#include <algorithm>
#include <vector>

SnakeGame::SnakeGame()
{
    std::cout << "[" << _name << "] Constructor called" << std::endl;

    this->set_elapsed(40); //otherwise snake goes way too fast

    int start_x = WIDTH / 2;
    int start_y = HEIGHT / 2;

    _snake.push_back({start_x, start_y});
    generateFood();

}

const std::string &SnakeGame::getName() const
{
    return _name;
}

bool SnakeGame::checkCollision(int x, int y)
{
    //check for coll with walls
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return true;
    }
    //now we check coll with itself
    auto head = _snake.front();
    for (size_t i = 1; i < _snake.size(); i++) {
        if (_snake[i] == head) {
            return true;
        }
    }
    return false;
}

void SnakeGame::reset_game(EventType input)
{
    _display->clear();
    _display->drawText("Game Over! Press SPACE to restart", WIDTH / 2 - 10, HEIGHT / 2);
    _display->drawText("Score: " + std::to_string(_score), WIDTH / 2 - 10, HEIGHT / 2 + 1);
    _display->drawText("High Score: " + std::to_string(_highscore), WIDTH / 2 - 10, HEIGHT / 2 + 2);

    _display->draw();

    if (input == SPACE_KEY) {
        if (_score > _highscore) {
            set_highscore(_score);
        }
        _score = 0;
        _snake.clear();
        int start_x = WIDTH / 2;
        int start_y = HEIGHT / 2;
        _snake.push_back({start_x, start_y});
        generateFood();
        _gameover = false;
    }
}


//for now we'll assume that the width and height of the game is 10*10 but in the future display and width will need to be given by the display module, and the game will need to adapt to it, but for now we'll just hardcode it for testing purposes.
void SnakeGame::tick(EventType input)
{
    //first we check for game over
    if (_gameover) {
        reset_game(input);
        return;
    }


    if (input == W_KEY && _currentDir != DOWN)
        _nextDir = UP;
    if (input == A_KEY && _currentDir != RIGHT)
        _nextDir = LEFT;
    if (input == S_KEY && _currentDir != UP)
        _nextDir = DOWN;
    if (input == D_KEY && _currentDir != LEFT)
        _nextDir = RIGHT;

    _currentDir = _nextDir;

    auto [head_x, head_y] = _snake.front(); //we get the head of the snake, which is the front of the deque, and we use it to check for collisions with the food and with the snake itself, and we use it to update the game map based on the snake's movement

    switch (_currentDir) { //update head position based on curr pos
        case UP:
            head_y--;
            break;
        case DOWN:
            head_y++;
            break;
        case LEFT:
            head_x--;
            break;
        case RIGHT:
            head_x++;
            break;
    }

    //check for coll with walls
    if (checkCollision(head_x, head_y)) {
        _gameover = true;
        return;
    }

    bool ateFood = (head_x == _foodPos.first && head_y == _foodPos.second);

    _snake.push_front({head_x, head_y}); //we add the new head pos

    if (ateFood) {
        _score++;
        generateFood(); //we generate new food if we ate the food
    } else
        _snake.pop_back(); //we remove tail snake if we not eat food

    //update map. //we could actually directly call drawtile func on this
    _display->clear();
    for (const auto& seg : _snake) {
        _display->drawTile(SQUARE, GREEN, seg.first, seg.second);
    }
    _display->drawTile(CIRCLE, RED, _foodPos.first, _foodPos.second);
}

void SnakeGame::generateFood()
{
    int food_x, food_y;
    do {
        food_x = rand() % WIDTH;
        food_y = rand() % HEIGHT;
    } while (std::find(_snake.begin(), _snake.end(), std::make_pair(food_x, food_y)) != _snake.end()); //we need to make sure that the food is not generated on the snake

    _foodPos = {food_x, food_y};
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