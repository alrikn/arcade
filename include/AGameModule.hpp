/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** AGameModule
*/

#ifndef INCLUDED_AGAMEMODULE_HPP
    #define INCLUDED_AGAMEMODULE_HPP

#include "IGameModule.hpp"

class AGameModule : public IGameModule
{
    private:
    protected:
        unsigned long _elapsed = 16;// time in millisecond between ticks.

    public:

        unsigned long get_elapsed() override
        {
            return _elapsed;
        }

        void set_elapsed(unsigned long elapsed) override
        {
            _elapsed = elapsed;
        }

};


#endif
