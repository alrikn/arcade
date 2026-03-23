/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Ncurses
*/

#ifndef INCLUDED_NCURSES_HPP
    #define INCLUDED_NCURSES_HPP

#include "ADisplayModule.hpp"
#include "IDisplayModule.hpp"
#include <map>

const std::map<ShapeType, char> shapeToChar = {
    {CIRCLE, 'O'},
    {SQUARE, '#'},
    {EMPTY, ' '}
};

class Ncurses: public ADisplayModule
{
    private:
        const std::string _name = "Ncurses";

    public:
        Ncurses() = default;

        ~Ncurses() = default;

        void init(void) override;
        void stop(void) override;
        const std::string &getName() const override;

        void draw() override;
        void clear() override;
        EventType pollEvents() override;
        void drawText(const std::string& text, int x, int y) override;
        void drawTile(ShapeType shape, Color color, int x, int y) override;
        void drawSprite(const Sprite &sprite, int x, int y) override;

        int getNcurseWidth();
        int getNcurseHeight();

        int get_color_pair(Color color);


        int _originX = 0;
        int _originY = 0;

        void updateLayout();
        void drawBorder();

};


#endif
