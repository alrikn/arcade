/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Ncurses
*/

#include "Ncurses.hpp"
#include "IDisplayModule.hpp"
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
    nodelay(stdscr, TRUE); //make getch non-blocking

    //init the color pairs

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

int Ncurses::get_color_pair(Color color)
{
    switch (color) {
        case RED:
            return 1;
        case GREEN:
            return 2;
        case BLUE:
            return 3;
        case YELLOW:
            return 4;
        case MAGENTA:
            return 5;
        case CYAN:
            return 6;
        case WHITE:
            return 7;
        default:
            return 0; //default color pair (black on black)
    }
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
    const int termW = getNcurseWidth();
    const int termH = getNcurseHeight();

    if (termW < static_cast<int>(_width) + 2 || termH < static_cast<int>(_height) + 2) {
        erase();
        const std::string msg1 = "Please resize your terminal to at least " +
            std::to_string(_width + 2) + "x" + std::to_string(_height + 2);
        const std::string msg2 = "Current size is " + std::to_string(termW) + "x" + std::to_string(termH);

        mvprintw(termH / 2, std::max(0, (termW - static_cast<int>(msg1.size())) / 2), "%s", msg1.c_str());
        mvprintw(termH / 2 + 1, std::max(0, (termW - static_cast<int>(msg2.size())) / 2), "%s", msg2.c_str());
        refresh();
        return;
    }

    updateLayout();
    drawBorder();
    refresh();
}

void Ncurses::clear()
{
    //clear the screen for the next draw
    //this is calling itself insteasd of the ncurses clear function, we need to call the ncurses clear function instead
    ::erase();
}

EventType Ncurses::pollEvents()
{

    //we can use getch() to get input, and we can use a switch statement to convert it to our EventType enum
    int ch = getch();
    switch (ch) {
        case 'w':
        case KEY_UP:
            return W_KEY;
        case 'a':
        case KEY_LEFT:
            return A_KEY;
        case 's':
        case KEY_DOWN:
            return S_KEY;
        case 'd':
        case KEY_RIGHT:
            return D_KEY;
        case ' ':
            return SPACE_KEY;
        case KEY_MOUSE:
            //we would need to use getmouse() to get the mouse event, but for now we'll just push back a generic mouse event
            return MOUSE_L; //we'll just return MOUSE_L for now, but in the future we will need to differentiate between left and right mouse clicks, and we will need to get the position of the mouse click as well
        case 'q': //we also need to take in esc
        case 27: //esc key
            return QUIT;
        default:
            return OTHER;
    }
}



void Ncurses::drawText(const std::string& text, int x, int y)
{
    //move the cursor to the given position and print the text
    mvprintw(y + _originY, x + _originX, "%s", text.c_str());
}


void Ncurses::drawTile(ShapeType shape, Color color, int x, int y)
{
    //we can use the shapeToChar map to get the character to draw based on the shape
    char tileChar = shapeToChar.at(shape);

    //now for the color

    //the color could be used to set the color pair in ncurses, but for now we'll ignore it
    attron(COLOR_PAIR(get_color_pair(color)));
    mvaddch(y + _originY, x + _originX, tileChar);
    attroff(COLOR_PAIR(get_color_pair(color)));
}

void Ncurses::drawSprite(const Sprite &sprite, int x, int y)
{
    (void)sprite;
    attron(COLOR_PAIR(get_color_pair(WHITE)));
    mvaddch(y + _originY, x + _originX, '@');
    attroff(COLOR_PAIR(get_color_pair(WHITE)));
}

int Ncurses::getNcurseHeight()
{
    unsigned int width, height;
    getmaxyx(stdscr, height, width);
    return height;
}

int Ncurses::getNcurseWidth()
{
    unsigned int width, height;
    getmaxyx(stdscr, height, width);
    return width;
}

void Ncurses::updateLayout()
{
    _originX = (getNcurseWidth() - static_cast<int>(_width)) / 2;
    _originY = (getNcurseHeight() - static_cast<int>(_height)) / 2;
}

void Ncurses::drawBorder()
{
    const int x = _originX - 1;
    const int y = _originY - 1;

    attron(COLOR_PAIR(get_color_pair(WHITE)));

    mvaddch(y, x, ACS_ULCORNER);
    mvhline(y, x + 1, ACS_HLINE, _width);
    mvaddch(y, x + _width + 1, ACS_URCORNER);

    mvvline(y + 1, x, ACS_VLINE, _height);
    mvvline(y + 1, x + _width + 1, ACS_VLINE, _height);

    mvaddch(y + _height + 1, x, ACS_LLCORNER);
    mvhline(y + _height + 1, x + 1, ACS_HLINE, _width);
    mvaddch(y + _height + 1, x + _width + 1, ACS_LRCORNER);

    attroff(COLOR_PAIR(get_color_pair(WHITE)));
}

//C interface (THIS is what dlopen/dlsym uses)
extern "C" {

IDisplayModule* create()
{
    return new Ncurses();
}

void destroy(IDisplayModule* instance)
{
    instance->stop(); //we call stop before deleting the instance to make sure we end ncurses mode properly
    delete instance;
}

}
