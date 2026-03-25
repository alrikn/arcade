/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** TetrisGame
*/

#include "TetrisGame.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

// pieces and each rotation variaiton, to return a 2d matrix of 4x4
const int TetrisGame::SHAPES[SHAPE_COUNT][ROTATION_COUNT][SHAPE_SIZE][SHAPE_SIZE] = {
    // I
    {
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}
    },
    // O
    {
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    },
    // T
    {
        {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}
    },
    // S
    {
        {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}
    },
    // Z
    {
        {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}
    },
    // J
    {
        {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}
    },
    // L
    {
        {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}
    }
};

const Color TetrisGame::SHAPE_COLORS[SHAPE_COUNT] = {
    CYAN,
    YELLOW,
    MAGENTA,
    GREEN,
    RED,
    BLUE,
    WHITE
};

TetrisGame::TetrisGame()
{
    std::cout << "[" << _name << "] constructor called" << std::endl;
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // each run the piece sequence will be diff

    _offsetX = (WIDTH - BOARD_WIDTH) / 2;
    _offsetY = (HEIGHT - BOARD_HEIGHT) / 2;

    set_elapsed(50);
    _lastFallTime = std::chrono::steady_clock::now();
    initBoard();
    spawnPiece();
}

const std::string &TetrisGame::getName() const
{
    return _name;
}

void TetrisGame::initBoard()
{
    _board.assign(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
}

void TetrisGame::spawnPiece()
{
    _currentShape = std::rand() % SHAPE_COUNT;
    _currentRotation = 0;
    _currentX = (BOARD_WIDTH / 2) - 2;
    _currentY = 0;

    if (!canPlace(_currentShape, _currentRotation, _currentX, _currentY))
        _gameover = true;
}

bool TetrisGame::canPlace(int shape, int rotation, int x, int y) const
{
    for (int row = 0; row < SHAPE_SIZE; ++row) {
        for (int col = 0; col < SHAPE_SIZE; ++col) {
            if (SHAPES[shape][rotation][row][col] == 0)
                continue;

            int bx = x + col;
            int by = y + row;

            if (bx < 0 || bx >= BOARD_WIDTH || by < 0 || by >= BOARD_HEIGHT)
                return false;

            if (_board[by][bx] != 0)
                return false;
        }
    }
    return true;
}

void TetrisGame::moveCurrent(int dx, int dy)
{
    int nx = _currentX + dx;
    int ny = _currentY + dy;

    if (canPlace(_currentShape, _currentRotation, nx, ny)) {
        _currentX = nx;
        _currentY = ny;
        return;
    }

    // if it cant move down, the piece is fixed to the board
    if (dy == 1) {
        lockCurrentPiece();
        clearFullLines();
        spawnPiece();
    }
}

void TetrisGame::rotateCurrentRight()
{
    int nextRotation = (_currentRotation + 1) % ROTATION_COUNT;
    if (canPlace(_currentShape, nextRotation, _currentX, _currentY))
        _currentRotation = nextRotation;
}

void TetrisGame::lockCurrentPiece()
{
    for (int row = 0; row < SHAPE_SIZE; ++row) {
        for (int col = 0; col < SHAPE_SIZE; ++col) {
            if (SHAPES[_currentShape][_currentRotation][row][col] == 0)
                continue;

            int bx = _currentX + col;
            int by = _currentY + row;
            if (bx >= 0 && bx < BOARD_WIDTH && by >= 0 && by < BOARD_HEIGHT)
                _board[by][bx] = 1;
        }
    }
}

void TetrisGame::clearFullLines()
{
    for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (_board[y][x] == 0) {
                full = false;
                break;
            }
        }

        if (!full)
            continue;

        _board.erase(_board.begin() + y);
        _board.insert(_board.begin(), std::vector<int>(BOARD_WIDTH, 0));
        y++; // re check same line index after collapse bc maybe the line created is full
    }
}

void TetrisGame::render()
{
    if (!_display)
        return;

    _display->clear();

    // draw border
    for (int x = -1; x <= BOARD_WIDTH; ++x) {
        _display->drawText("-", _offsetX + x, _offsetY - 1);
        _display->drawText("-", _offsetX + x, _offsetY + BOARD_HEIGHT);
    }
    _display->drawText("+", _offsetX - 1, _offsetY - 1);
    _display->drawText("+", _offsetX + BOARD_WIDTH, _offsetY - 1);
    _display->drawText("+", _offsetX - 1, _offsetY + BOARD_HEIGHT);
    _display->drawText("+", _offsetX + BOARD_WIDTH, _offsetY + BOARD_HEIGHT);
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        _display->drawText("|", _offsetX - 1, _offsetY + y);
        _display->drawText("|", _offsetX + BOARD_WIDTH, _offsetY + y);
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (_board[y][x] != 0)
                _display->drawText("#", _offsetX + x, _offsetY + y);
        }
    }

    if (!_gameover) {
        for (int row = 0; row < SHAPE_SIZE; ++row) {
            for (int col = 0; col < SHAPE_SIZE; ++col) {
                if (SHAPES[_currentShape][_currentRotation][row][col] == 0)
                    continue;
                _display->drawText("@", _offsetX + _currentX + col, _offsetY + _currentY + row);
            }
        }
    }

    _display->drawText("tetris: left/right move, up rotate, down soft drop", _offsetX - 6, _offsetY + BOARD_HEIGHT + 1);
    if (_gameover)
        _display->drawText("GAME OVER", _offsetX + 1, _offsetY + (BOARD_HEIGHT / 2));
}

void TetrisGame::tick(EventType input)
{
    if (_gameover) {
        render();
        return;
    }

    if (input == A_KEY)
        moveCurrent(-1, 0);
    else if (input == D_KEY)
        moveCurrent(1, 0);
    else if (input == S_KEY)
        moveCurrent(0, 1);
    else if (input == W_KEY)
        rotateCurrentRight();

    // gravity now time based
    auto now = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFallTime).count();
    if (elapsedMs >= _fallIntervalMs) {
        moveCurrent(0, 1);
        _lastFallTime = now;
    }
    render();
}

void TetrisGame::exit()
{
    std::cout << "[" << _name << "] exit called" << std::endl;
}

extern "C" {

IGameModule *create()
{
    return new TetrisGame();
}

void destroy(IGameModule *instance)
{
    delete instance;
}

}
