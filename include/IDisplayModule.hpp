/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** IDisplayModule
*/

#ifndef INCLUDED_IDISPLAYMODULE_HPP
    #define INCLUDED_IDISPLAYMODULE_HPP


#include <string>
#include <vector>

enum ShapeType {
    EMPTY = -1, //empty is a valid shape type.
    CIRCLE,
    SQUARE,
};

enum EventType {
    OTHER = -1, //we will return other, but we won't tell you what it is
    W_KEY, //also is the arrow up key
    A_KEY, //also is the arrow left key
    S_KEY, //also is the arrow down key
    D_KEY, //also is the arrow right key
    SPACE_KEY,
    MOUSE_L,
    MOUSE_R,
    MENU, //so that the core can detect when we want to go to the menu
    QUIT,
};

class IDisplayModule {
    public:
        virtual ~IDisplayModule() = default;
        virtual void init() = 0;
        virtual void stop() = 0;
        virtual const std::string &getName() const = 0;
        virtual void draw() = 0; //draws the current state of the game
        virtual void clear() = 0; //clears the screen (for the next draw)

        virtual EventType pollEvents() = 0; //polls for events (keyboard, mouse, etc.)
        //however, the pollEvents is not enough, we also need a way to get the events that were polled
        // we can do this by having a getEvents function that returns a vector of events that were polled.


        //drawing functions (these are just examples, we can add more as needed)
        virtual void drawText(const std::string& text, int x, int y) = 0; //draws text at the given position

        //we need to keep in mind that while sfml can take a sprite object, ncurses can only draw characters
        //we could have a simple sprite class that can be used by both libraries, and the drawSprite function can take care of the differences between the two libraries
        virtual void drawTile(ShapeType shape, int color, int x, int y) = 0; //draws a space at the given position (for clearing)

        //get width and height of display
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;

};
#endif
