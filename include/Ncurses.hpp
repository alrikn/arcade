/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Ncurses
*/

#ifndef INCLUDED_NCURSES_HPP
    #define INCLUDED_NCURSES_HPP

#include "IDisplayModule.hpp"
#include <map>

const std::map<ShapeType, char> shapeToChar = {
    {CIRCLE, 'O'},
    {SQUARE, '#'},
    {EMPTY, ' '}
};

class Ncurses: public IDisplayModule
{
    private:
        const std::string _name = "Ncurses";
        std::vector<EventType> _events; //we will store the events that were polled here, and getEvents will return this vector

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

        int getWidth() override;
        int getHeight() override;

        int get_color_pair(Color color);

};


#endif
