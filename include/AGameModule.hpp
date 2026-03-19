/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** AGameModule
*/

#ifndef INCLUDED_AGAMEMODULE_HPP
    #define INCLUDED_AGAMEMODULE_HPP

#include "IDisplayModule.hpp"
#include "IGameModule.hpp"

class AGameModule : public IGameModule
{
    private:
    protected:
        //this equates t
        unsigned long _elapsed = 25;// time in millisecond between ticks.
        unsigned int _score = 0;
        unsigned int _highscore = 0;
        bool _gameover = false;
        IDisplayModule* _display = nullptr;

    public:

        AGameModule() = default;
        ~AGameModule() = default;

        unsigned long get_elapsed() override
        {
            return _elapsed;
        }

        unsigned int get_score() override
        {
            return _score;
        }

        void set_score(unsigned int score) override
        {
            _score = score;
        }

        void set_elapsed(unsigned long elapsed) override
        {
            _elapsed = elapsed;
        }

        bool get_gameover() override
        {
            return _gameover;
        }

        void load_display(IDisplayModule* display) override
        {
            _display = display;
            _display->init();

        }

};


#endif
