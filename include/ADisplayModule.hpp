/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** ADisplayModule
*/

#ifndef INCLUDED_ADISPLAYMODULE_HPP
    #define INCLUDED_ADISPLAYMODULE_HPP

#include "IDisplayModule.hpp"
#include <iostream>

class ADisplayModule : public IDisplayModule
{
    protected:
        const unsigned int _width = 60; //default width
        const unsigned int _height = 40; //default height
    public:

        int getWidth() override
        {
            return _width;
        }

        int getHeight() override
        {
            return _height;
        }
};


#endif
