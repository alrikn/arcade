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

    // core handles pacing: each game tick advances gravity once
    set_elapsed(250);
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

    if (!canPlace(_currentShape, _currentRotation, _currentX, _currentY)) {
        _gameover = true;
        if (get_score() > get_highscore())
            set_highscore(get_score());
    }
}

void TetrisGame::resetGame()
{
    _gameover = false;
    set_score(0);
    initBoard();
    spawnPiece();
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
        int linesCleared = clearFullLines();
        if (linesCleared > 0) {
            static const unsigned int lineScore[5] = {0, 100, 300, 500, 800};
            unsigned int gained = lineScore[(linesCleared > 4) ? 4 : linesCleared];
            set_score(get_score() + gained);
            if (get_score() > get_highscore())
                set_highscore(get_score());
        }
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
                _board[by][bx] = _currentShape + 1;
        }
    }
}

int TetrisGame::clearFullLines()
{
    int cleared = 0;

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
        cleared++;
        y++; // re check same line index after collapse bc maybe the line created is full
    }

    return cleared;
}

void TetrisGame::render()
{
    if (!_display)
        return;

    _display->clear();

    // draw border
    for (int x = -1; x <= BOARD_WIDTH; ++x) {
        _display->drawText("-", WHITE, _offsetX + x, _offsetY - 1);
        _display->drawText("-", WHITE, _offsetX + x, _offsetY + BOARD_HEIGHT);
    }
    _display->drawText("+", WHITE, _offsetX - 1, _offsetY - 1);
    _display->drawText("+", WHITE, _offsetX + BOARD_WIDTH, _offsetY - 1);
    _display->drawText("+", WHITE, _offsetX - 1, _offsetY + BOARD_HEIGHT);
    _display->drawText("+", WHITE, _offsetX + BOARD_WIDTH, _offsetY + BOARD_HEIGHT);
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        _display->drawText("|", WHITE, _offsetX - 1, _offsetY + y);
        _display->drawText("|", WHITE, _offsetX + BOARD_WIDTH, _offsetY + y);
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            int cell = _board[y][x];
            if (cell <= 0)
                continue;
            Color cellColor = WHITE;
            if (cell <= SHAPE_COUNT)
                cellColor = SHAPE_COLORS[cell - 1];
            _display->drawTile(SQUARE, cellColor, _offsetX + x, _offsetY + y);
        }
    }

    if (!_gameover) {
        for (int row = 0; row < SHAPE_SIZE; ++row) {
            for (int col = 0; col < SHAPE_SIZE; ++col) {
                if (SHAPES[_currentShape][_currentRotation][row][col] == 0)
                    continue;
                _display->drawTile(SQUARE, SHAPE_COLORS[_currentShape], _offsetX + _currentX + col, _offsetY + _currentY + row);
            }
        }
    }

    _display->drawText("TETRIS", WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 1);
    _display->drawText("A/D: move", WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 2);
    _display->drawText("W: rotate", WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 3);
    _display->drawText("S: hard drop", WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 4);
    _display->drawText("Score: " + std::to_string(get_score()), WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 5);
    _display->drawText("High: " + std::to_string(get_highscore()), WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 6);
    if (_gameover) {
        _display->drawText("GAME OVER", RED, _offsetX + 1, _offsetY + (BOARD_HEIGHT / 2));
        _display->drawText("SPACE: restart", WHITE, _offsetX, _offsetY + BOARD_HEIGHT + 7);
    }
}

void TetrisGame::tick(EventType input)
{
    if (_gameover) {
        if (input == SPACE_KEY)
            resetGame();
        render();
        return;
    }

    bool shouldApplyGravity = true;

    if (input == A_KEY)
        moveCurrent(-1, 0);
    else if (input == D_KEY)
        moveCurrent(1, 0);
    else if (input == S_KEY) {
        while (canPlace(_currentShape, _currentRotation, _currentX, _currentY + 1))
            _currentY++;
        moveCurrent(0, 1); // lock + clear + spawn through existing path
        shouldApplyGravity = false;
    }
    else if (input == W_KEY)
        rotateCurrentRight();

    if (shouldApplyGravity)
        moveCurrent(0, 1);
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
