/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** ADisplayModule
*/

#ifndef INCLUDED_ADISPLAYMODULE_HPP
    #define INCLUDED_ADISPLAYMODULE_HPP

#include "IDisplayModule.hpp"
#include <iostream>

class ADisplayModule : public IDisplayModule
{
    protected:
        const unsigned int _width = WIDTH; //default width
        const unsigned int _height = HEIGHT; //default height
    public:

        int getWidth() override
        {
            return _width;
        }

        int getHeight() override
        {
            return _height;
        }

        virtual char getInputChar() override
        {
            return '\n'; //by default, we return enter character, which means we skip the naming
        }

        virtual void drawSprite(const Sprite &sprite, int x, int y) override
        {
            drawTile(sprite.fallback, sprite.fallbackColor, x, y);
        }
};


#endif
