/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Ncurses
*/

#include "Ncurses.hpp"
#include <iostream>
#include <ncurses.h>


void Ncurses::init(void)
{
    //now we initialize ncurses
    initscr(); //initialize the screen
    cbreak(); //disable line buffering
    noecho(); //don't echo input
    keypad(stdscr, TRUE); //enable special keys (like arrow keys)
    curs_set(0); //hide the cursor
}


void Ncurses::stop(void)
{
    endwin(); //end ncurses mode
}

const std::string &Ncurses::getName() const
{
    return _name;
}

void Ncurses::draw()
{
    refresh(); //refresh the screen to show the changes
}

void Ncurses::clear()
{
    //clear the screen for the next draw
    clear();
}

void Ncurses::pollEvents()
{
    //for now we just print a message, but this is where we would poll for events
    std::cout << "[" << _name << "] pollEvents" << std::endl;

    //we can use getch() to get input, and we can use a switch statement to convert it to our EventType enum
    int ch = getch();
    switch (ch) {
        case 'w':
        case KEY_UP:
            _events.push_back(W_KEY);
            break;
        case 'a':
        case KEY_LEFT:
            _events.push_back(A_KEY);
            break;
        case 's':
        case KEY_DOWN:
            _events.push_back(S_KEY);
            break;
        case 'd':
        case KEY_RIGHT:
            _events.push_back(D_KEY);
            break;
        case ' ':
            _events.push_back(SPACE_KEY);
            break;
        case KEY_MOUSE:
            //we would need to use getmouse() to get the mouse event, but for now we'll just push back a generic mouse event
            _events.push_back(MOUSE_L); //we'll just assume it's a left click for now
            break;
        default:
            _events.push_back(OTHER);
            break;
    }
}

std::vector<EventType> Ncurses::getEvents()
{
    //we return the events that were polled, and then we clear the vector for the next poll
    std::vector<EventType> events = _events;
    _events.clear();
    return events;
}

void Ncurses::drawText(const std::string& text, int x, int y)
{
    //move the cursor to the given position and print the text
    mvprintw(y, x, "%s", text.c_str());
}

void Ncurses::drawTile(ShapeType shape, int color, int x, int y)
{
    //we can use the shapeToChar map to get the character to draw based on the shape
    char tileChar = shapeToChar.at(shape);
    //the color could be used to set the color pair in ncurses, but for now we'll ignore it
    mvaddch(y, x, tileChar);
}


//C interface (THIS is what dlopen/dlsym uses)
extern "C" {

IDisplayModule* create()
{
    return new Ncurses();
}

void destroy(IDisplayModule* instance)
{
    delete instance;
}

}
