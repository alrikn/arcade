/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SDL2
*/

#include "SDL2.hpp"
#include "Error.hpp"
#include <SDL2/SDL_image.h>

// check if keycode is a printable ascii char for name input
static bool isPrintableAscii(SDL_Keycode key)
{
	return key >= 32 && key <= 126;
}

// sdl2 module cons
SDL2::SDL2() : _window(nullptr), _renderer(nullptr), _font(nullptr)
{
}

// sdl2 module destructor
SDL2::~SDL2()
{
	stop();
}
