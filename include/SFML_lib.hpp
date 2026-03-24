/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SFML_lib
*/

#ifndef INCLUDED_SFML_LIB_HPP
    #define INCLUDED_SFML_LIB_HPP

#include "IDisplayModule.hpp"
#include <iostream>
#include "ADisplayModule.hpp"
#include <SFML/Graphics.hpp>

class SFML_lib : public ADisplayModule
{
    private:
        const std::string _name = "SFML_lib";
        sf::RenderWindow _window;
        sf::Font _font;

        unsigned int _tileSize = 20;
        int _originX = 0;
        int _originY = 0;

        void updateLayout();
    public:
        SFML_lib();
        ~SFML_lib() = default;
        const std::string &getName() const override{return _name;}

        void draw() override;
        EventType pollEvents() override;

        void init() override;
        void stop() override;

        void clear() override;
        void drawTile(ShapeType shape, Color color, int x, int y) override;
        void drawText(const std::string &text, Color color, int x, int y) override;

};


#endif
