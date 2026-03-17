/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** LibFoo
*/

#ifndef INCLUDED_LIBFOO_HPP
    #define INCLUDED_LIBFOO_HPP
    #include "IDisplayModule.hpp"
#include <string>


class LibFoo : public IDisplayModule
{
    private:
        const std::string _name = "LibFoo";
    protected:
    public:
        LibFoo() = default;
        ~LibFoo() = default;

        void init() override;
        void stop() override;
        const std::string &getName() const override;

};


#endif
