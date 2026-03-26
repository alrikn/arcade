/*
** EPITECH PROJECT, 2026
** boostrap_arcade
** File descrption:
** Nibbler
*/

#include "Nibbler.hpp"
#include "IDisplayModule.hpp"
#include "IGameModule.hpp"
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

static const char *MAZE[HEIGHT] = {
    "############################################################",
    "#..........................................................#",
    "#.........o...................o...................o........#",
    "#..........................................................#",
    "#...#############.........................######.....###...#",
    "#...#o..........#........o................#............#...#",
    "#...#...........########...................................#",
    "#...............#...o..#.o.........o.......................#",
    "#...............#......#...................................#",
    "#...............#......#..................#............#...#",
    "#...#######............#...........###########.....#####...#",
    "#.............#........#.................#.................#",
    "#.............#..................#########.................#",
    "#....o........#.o............................o.............#",
    "#################..........................................#",
    "#....o.................................................o...#",
    "#.............................#########....................#",
    "#.....................#...............#....................#",
    "#..............#......#...............########.............#",
    "#..............#......#....................................#",
    "#.........o....#......#...........................o........#",
    "#..............#......#.......#............................#",
    "########.......#..............#.......#....................#",
    "#..............#..............#.......#............####....#",
    "#..............#......#################.............#......#",
    "#..............#......#.............................#......#",
    "#..............########.....o...................o...#......#",
    "#..............#....................................#......#",
    "#.......########...........#............###o#######.####...#",
    "#...#......................#............#o.......o#.#......#",
    "#...#....................o.#............#.#o#####.#.#......#",
    "#...#.............######...#............#.#.....#.#.#......#",
    "#..o#.............#....#...#.......o....#.#......o#.#......#",
    "#####.............#....#####............#.#########.#......#",
    "#...#.............#....#................#o.........o#......#",
    "#...######....#####....#####............#############......#",
    "#..........................................................#",
    "#.........o...................o...................o........#",
    "#..........................................................#",
    "############################################################",
};

Nibbler::Nibbler()
{
    srand(time(nullptr));
    std::cout << "[" << _name << "] Constructor called" << std::endl;
    this->set_elapsed(80);
    loadMap(rand() % 2);
    generateSnakeStart();
}

const std::string &Nibbler::getName() const
{
    return _name;
}

void Nibbler::tick(EventType input)
{
    if (_gameover) {
        reset_game(input);
        return;
    }

    if (_levelWon) {
        _display->clear();
        _display->drawText("You Won! Press space to restart", YELLOW,WIDTH / 2 - 10, HEIGHT / 2);
        _display->drawText("Score: " + std::to_string(_score), WHITE,WIDTH / 2 - 10, HEIGHT / 2 + 1);
        if (input == SPACE_KEY) {
            if (_score > _highscore)
                set_highscore(_score);
            _score = 0;
            _levelWon = false;
            loadMap(rand() % 2);
            generateSnakeStart();
        }
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
    auto [head_x, head_y] = _snake.front();
    int next_x = head_x;
    int next_y = head_y;

    switch (_currentDir) {
        case UP:
            next_y--;
            break;
        case DOWN:
            next_y++;
            break;
        case LEFT:
            next_x--;
            break;
        case RIGHT:
            next_x++;
            break;
    }
    // this is the part where the auto turning of the snake comes :))))
    if (_map[next_y][next_x] == CELL_WALL) {
        _currentDir = tryAutoTurn(head_x, head_y, _currentDir);
        _nextDir = _currentDir;
        next_x = head_x;
        next_y = head_y;
        switch (_currentDir) {
            case UP:
                next_y--;
                break;
            case DOWN:
                next_y++;
                break;
            case LEFT:
                next_x--;
                break;
            case RIGHT:
                next_x++;
                break;
        }
    }

    //check for coll with walls
    if (checkCollision(next_x, next_y)) {
        _gameover = true;
        return;
    }

    bool ateFood = (_map[next_y][next_x] == CELL_FOOD);

    _snake.push_front({next_x, next_y}); //we add the new head pos

    if (ateFood) {
        _map[next_y][next_x] = CELL_EMPTY;
        _foodCount--;
        _score++;
        if (_elapsed > 40)
            set_elapsed(_elapsed - 5);
    } else
        _snake.pop_back(); //we remove tail snake if we not eat food

    if (_foodCount == 0) {
        _levelWon = true;
        return;
    }
    _display->clear();
    //for (int y = 0; y < HEIGHT; y++) {
    //    for (int x = 0; x < WIDTH; x++) {
    //        if (_map[y][x] == CELL_WALL)
    //            _display->drawTile(SQUARE, WHITE, x, y);
    //    }
    //}
    drawAssets();
}

void Nibbler::drawAssets()
{
    // food
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (_map[y][x] == CELL_FOOD) {
                Sprite food;
                food.fallback = CIRCLE; food.fallbackColor = RED;
                food.path = "snake/apple.png";
                _display->drawSprite(food, x, y);
            } else if (_map[y][x] == CELL_WALL) {
                Sprite wall;
                wall.fallback = SQUARE; wall.fallbackColor = WHITE;
                wall.path = "structure/wall.png";
                _display->drawSprite(wall, x, y);
            } else {
                Sprite floor;
                floor.fallback = EMPTY; floor.fallbackColor = BLACK;
                floor.path = "structure/floor.png";
                _display->drawSprite(floor, x, y);
            }
        }
    }

    Sprite head;
    head.fallback = SQUARE; head.fallbackColor = GREEN;
    if (_currentDir == UP)    head.path = "snake/head_up.png";
    if (_currentDir == DOWN)  head.path = "snake/head_down.png";
    if (_currentDir == LEFT)  head.path = "snake/head_left.png";
    if (_currentDir == RIGHT) head.path = "snake/head_right.png";
    _display->drawSprite(head, _snake[0].first, _snake[0].second);

    // body
    for (size_t i = 1; i < _snake.size(); i++) {
        Sprite body;
        body.fallback = SQUARE; body.fallbackColor = GREEN;
        bool horizontal = (_snake[i].second == _snake[i - 1].second);
        body.path = horizontal ? "snake/body_horizontal.png" : "snake/body_vertical.png";
        _display->drawSprite(body, _snake[i].first, _snake[i].second);
    }
}

void Nibbler::exit()
{
    std::cout << "[" << _name << "] exit called" << std::endl;
}

void Nibbler::reset_game(EventType input)
{
    _display->clear();
    _display->drawText("Game Over! Press SPACE to restart", RED,WIDTH / 2 - 10, HEIGHT / 2);
    _display->drawText("Score: " + std::to_string(_score), WHITE,WIDTH / 2 - 10, HEIGHT / 2 + 1);
    _display->drawText("High Score: " + std::to_string(_highscore), WHITE, WIDTH / 2 - 10, HEIGHT / 2 + 2);

    _display->draw();

    if (input == SPACE_KEY) {
        if (_score > _highscore)
            set_highscore(_score);
        _score = 0;
        _snake.clear();
        _gameover = false;
        _levelWon = false;
        loadMap(rand() % 2);
        generateSnakeStart();
    }
}

void Nibbler::loadMap(int rotation)
{
    char c;
    _rotation = rotation;

    _map.clear();
    _map.resize(HEIGHT, std::vector<CellType>(WIDTH, CELL_EMPTY));
    _foodCount = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (rotation == 1)
                c = MAZE[HEIGHT - 1 - y][WIDTH - 1 - x];
            else
                c = MAZE[y][x];
            if (c == '#')
                _map[y][x] = CELL_WALL;
            else if (c == 'o') {
                _map[y][x] = CELL_FOOD;
                _foodCount++;
            } else
                _map[y][x] = CELL_EMPTY;
        }
    }
}

void Nibbler::generateSnakeStart()
{
    _snake.clear();
    if (_rotation == 0) {
        _snake.push_back({4, 1}); // head
        _snake.push_back({3, 1});
        _snake.push_back({2, 1});
        _snake.push_back({1, 1}); // tail
        _currentDir = RIGHT;
    } else {
        _snake.push_back({WIDTH - 5, HEIGHT - 2}); // head
        _snake.push_back({WIDTH - 4, HEIGHT - 2});
        _snake.push_back({WIDTH - 3, HEIGHT - 2});
        _snake.push_back({WIDTH - 2, HEIGHT - 2}); // tail
        _currentDir = LEFT;
    }
    _nextDir = _currentDir;
}

bool Nibbler::checkCollision(int x, int y)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return true;
    if (_map[y][x] == CELL_WALL)
        return true;
    auto next = std::make_pair(x, y);
    for (size_t i = 1; i < _snake.size(); i++) {
        if (_snake[i] == next)
            return true;
    }
    return false;
}

Direction Nibbler::tryAutoTurn(int x, int y, Direction dir)
{
    Direction turnRight[4] = {RIGHT, LEFT, UP, DOWN};
    Direction turnLeft[4] = {LEFT, RIGHT, DOWN, UP};
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    Direction right = turnRight[dir];
    Direction left = turnLeft[dir];

    if (!checkCollision(x + dx[right], y + dy[right]))
        return right;
    if (!checkCollision(x + dx[left], y + dy[left]))
        return left;
    return dir;
}

extern "C" {

IGameModule* create()
{
    return new Nibbler();
}

void destroy(IGameModule* instance)
{
    delete instance;
}

}
