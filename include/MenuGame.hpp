/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** MenuGame
*/

#ifndef INCLUDED_MENUGAME_HPP
    #define INCLUDED_MENUGAME_HPP

#include "IGameModule.hpp"
#include <iostream>

class MenuGame : public IGameModule
{
    private:
    protected:
    public:
        MenuGame();
        ~MenuGame() = default;
        std::string get_path_chosen();

        const std::string &getName() const override;
        void load_display(IDisplayModule* display) override;
        void tick(EventType input) override;
        void exit() override;

};

#endif
