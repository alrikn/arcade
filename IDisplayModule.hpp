/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** IDisplayModule
*/

#ifndef INCLUDED_IDISPLAYMODULE_HPP
    #define INCLUDED_IDISPLAYMODULE_HPP


#include <string>

class IDisplayModule {
    public:
        virtual ~IDisplayModule() = default;
        virtual void init() = 0;
        virtual void stop() = 0;
        virtual const std::string &getName() const = 0;
};
#endif
