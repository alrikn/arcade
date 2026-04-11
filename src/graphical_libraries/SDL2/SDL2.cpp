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

// CHANGING COLORS HERE in sdl2 bc otherwise its undistingshable from sfml
static SDL_Color toSdlTextColor(Color color)
{
	static const std::map<Color, SDL_Color> textColorMap = {
		{RED, {255, 120, 120, 255}},
		{GREEN, {120, 255, 180, 255}},
		{BLUE, {120, 180, 255, 255}},
		{YELLOW, {255, 235, 140, 255}},
		{MAGENTA, {255, 170, 235, 255}},
		{CYAN, {140, 255, 255, 255}},
		{WHITE, {225, 255, 205, 255}},
		{BLACK, {15, 30, 45, 255}},
	};

	auto it = textColorMap.find(color);
	if (it != textColorMap.end())
		return it->second;
	return {15, 30, 45, 255};
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

// converts project colors to sdl colors
SDL_Color SDL2::toSdlColor(Color color)
{
	static const std::map<Color, SDL_Color> colorMap = {
		{RED, {255, 0, 0, 255}},
		{GREEN, {0, 255, 0, 255}},
		{BLUE, {0, 0, 255, 255}},
		{YELLOW, {255, 255, 0, 255}},
		{MAGENTA, {255, 0, 255, 255}},
		{CYAN, {0, 255, 255, 255}},
		{WHITE, {255, 255, 255, 255}},
		{BLACK, {0, 0, 0, 255}},
	};

	auto it = colorMap.find(color);
	if (it != colorMap.end())
		return it->second;
	return {0, 0, 0, 255};
}
