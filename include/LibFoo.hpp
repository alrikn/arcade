/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** LibFoo
*/

#ifndef INCLUDED_LIBFOO_HPP
    #define INCLUDED_LIBFOO_HPP
    #include "IDisplayModule.hpp"
#include <string>


class LibFoo : public IDisplayModule
{
    private:
        const std::string _name = "LibFoo";
    protected:
    public:
        LibFoo() = default;
        ~LibFoo() = default;

        void init(void) override;
        void stop(void) override;
        const std::string &getName() const override;

        void draw() override;
        void clear() override;
        EventType pollEvents() override;
        void drawText(const std::string& text, int x, int y) override;
        void drawTile(ShapeType shape, Color color, int x, int y) override;
        void drawSprite(const Sprite &sprite, int x, int y) override;
        
        int getWidth() override;
        int getHeight() override;

};


#endif
