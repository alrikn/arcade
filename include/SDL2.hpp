/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SDL2
*/

#ifndef INCLUDED_SDL2_HPP
    #define INCLUDED_SDL2_HPP

#include "ADisplayModule.hpp"
#include "IDisplayModule.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <map>
#include <set>
#include <string>
#include <iostream>

class SDL2 : public ADisplayModule
{
    private:
        const std::string _name = "SDL2";
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        TTF_Font *_font;
        std::map<std::string, SDL_Texture*> _textures;
        std::set<std::string> _failedTextures;

        unsigned int _tileSize = 20;
        int _originX = 0;
        int _originY = 0;
    protected:
    public:
        SDL2();
        ~SDL2();
        const std::string &getName() const override{return _name;}
        
        void init() override;
        void stop() override;
        void draw() override;
        void clear() override;
        EventType pollEvents() override;

        void drawTile(ShapeType shape, Color color, int x, int y) override;
        void drawText(const std::string &text, Color color, int x, int y) override;
        void drawSprite(const Sprite &sprite, int x, int y) override;
};

#endif
