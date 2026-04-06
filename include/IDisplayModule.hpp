/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** IDisplayModule
*/

#ifndef INCLUDED_IDISPLAYMODULE_HPP
    #define INCLUDED_IDISPLAYMODULE_HPP


#include <cstdint>
#include <string>
#include <vector>

enum ShapeType {
    EMPTY = -1, //empty is a valid shape type.
    CIRCLE,
    SQUARE,
    UNKNOWN, // fallback for sprites without a defined shape
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
    ENTER,
    NUM_1,
    NUM_2,
    QUIT,
};

enum Color {
    BLACK = 0, //black is a valid color, but we will use it to represent the absence of color, so we can use it to represent empty tiles in the game map
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    WHITE,
};

const int HEIGHT = 40;
const int WIDTH = 60;

struct Sprite {
    std::string path; // path relative to assets
    uint8_t srcX = 0; // source rect X in pixels (offset)
    uint8_t srcY = 0; // source rect Y in pixels (offset)
    uint8_t srcW = 0; // source rect width in pixels
    uint8_t srcH = 0; // source rect height in pixels
    ShapeType fallback = UNKNOWN; // fallback shape for non-graphical libraries
    Color fallbackColor = WHITE; // fallback color for non-graphical libraries
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
        virtual void drawText(const std::string& text, Color color, int x, int y) = 0; //draws text at the given position

        //we need to keep in mind that while sfml can take a sprite object, ncurses can only draw characters
        //we could have a simple sprite class that can be used by both libraries, and the drawSprite function can take care of the differences between the two libraries
        virtual void drawTile(ShapeType shape, Color color, int x, int y) = 0; //draws a space at the given position (for clearing)
        virtual void drawSprite(const Sprite &sprite, int x, int y) = 0;

        //get width and height of display
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;

        virtual char getInputChar() = 0; //get the last input character (for text input)

};
#endif
