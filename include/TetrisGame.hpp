/*
** EPITECH PROJECT, 2026
** arcade
** File description:
** TetrisGame
*/

#ifndef INCLUDED_TETRISGAME_HPP
    #define INCLUDED_TETRISGAME_HPP

#include "AGameModule.hpp"
#include <vector>
#include <string>

class TetrisGame : public AGameModule
{
    public:
        TetrisGame();
        ~TetrisGame() = default;

        const std::string &getName() const override;
        void tick(EventType input) override;
        void exit() override;

    private:
        static constexpr int BOARD_WIDTH = 10;
        static constexpr int BOARD_HEIGHT = 20;

        static constexpr int SHAPE_COUNT = 7;
        static constexpr int ROTATION_COUNT = 4;
        static constexpr int SHAPE_SIZE = 4;

        const std::string _name = "TetrisGame";

        std::vector<std::vector<int>> _board;

        int _currentShape = 0;
        int _currentRotation = 0;
        int _currentX = 0;
        int _currentY = 0;

        int _offsetX = 0;
        int _offsetY = 0;

        void initBoard();
        void spawnPiece();
        bool canPlace(int shape, int rotation, int x, int y) const;
        void moveCurrent(int dx, int dy);
        void rotateCurrentRight();
        void lockCurrentPiece();
        void clearFullLines();
        void render();

        static const int SHAPES[SHAPE_COUNT][ROTATION_COUNT][SHAPE_SIZE][SHAPE_SIZE];
};

#endif
