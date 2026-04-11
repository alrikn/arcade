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

// loads and caches textures from assets/
SDL_Texture *SDL2::loadTexture(const std::string &path)
{
	if (!_renderer)
		return nullptr;

	const std::string fullPath = "assets/" + path;

	auto it = _textures.find(fullPath);
	if (it != _textures.end())
		return it->second;

	if (_failedTextures.count(fullPath))
		return nullptr;

	SDL_Surface *surface = IMG_Load(fullPath.c_str());
	if (!surface) {
		_failedTextures.insert(fullPath);
		return nullptr;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		_failedTextures.insert(fullPath);
		return nullptr;
	}
	_textures[fullPath] = texture;
	return texture;
}

// init sdl2, SDL_image, SDL_ttf and window resources:
// sources: https://thenumb.at/cpp-course/sdl2/01/01.html
// and https://thenumb.at/cpp-course/sdl2/05/05.html
void SDL2::init()
{
	if (_window && _renderer)
		return;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw GraphicalError("sdl init failed: " + std::string(SDL_GetError()));
	const int imageFlags = IMG_INIT_PNG;
	if ((IMG_Init(imageFlags) & imageFlags) == 0) {
		SDL_Quit();
		throw GraphicalError("SDL_image init failed: " + std::string(IMG_GetError()));
	}
	if (TTF_Init() == -1) {
		IMG_Quit();
		SDL_Quit();
		throw GraphicalError("SDL_ttf init failed: " + std::string(TTF_GetError()));
	}

	const int winW = static_cast<int>(_width * _tileSize + 2 * _tileSize);
	const int winH = static_cast<int>(_height * _tileSize + 2 * _tileSize);

	_window = SDL_CreateWindow("Arcade - SDL2", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, winW, winH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!_window) {
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		throw GraphicalError("sdl window creation failed: " + std::string(SDL_GetError()));
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) {
		SDL_DestroyWindow(_window);
		_window = nullptr;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		throw GraphicalError("sdl renderer creation failed: " + std::string(SDL_GetError()));
	}

	_font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 20);
	if (!_font)
		_font = TTF_OpenFont("assets/Xolonium-Regular.ttf", 18);
	if (!_font) {
		stop();
		throw GraphicalError("failed to load dont for sdl renderer");
	}

	_originX = static_cast<int>(_tileSize);
	_originY = static_cast<int>(_tileSize);
}

// c entry point used by the dynamic loader to create the sdl2 module no other choice anyway
extern "C" {

IDisplayModule* create()
{
	return new SDL2();
}

// c entry point used by the dynamic loader to destroy the sdl2 module no other choice anyway
void destroy(IDisplayModule* instance)
{
	instance->stop();
	delete instance;
}

}