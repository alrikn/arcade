/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** IGameModule
*/

#ifndef INCLUDED_IGAMEMODULE_HPP
    #define INCLUDED_IGAMEMODULE_HPP

#include "IDisplayModule.hpp"
#include <iostream>

class IGameModule
{
    protected:
    public:
        virtual ~IGameModule() = default;
        virtual const std::string &getName() const = 0;

        //a game needs to get events from the display.
        virtual void load_display(IDisplayModule* display) = 0; //this is how the game gets the display module, it can then use this to get events and draw things

        //I don't yet know where the game exit logic will be stored, but if it needs to be handled by the game, we'll use loop, and if its handled by the core, we'll use tick.
        virtual void tick(EventType input) = 0; //this is where the game logic goes, it will be called every frame by the main loop
        //loop is actually handled by the core
        virtual void exit() = 0; //when the core needs the game to exit, it will call this function, and the game can do any cleanup it needs to do here before the core unloads the library
};

#endif
