/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** Error
*/

#ifndef INCLUDED_ERROR_HPP
    #define INCLUDED_ERROR_HPP

#include <exception>
#include <string>



class Error: public std::exception
{
    protected:
        std::string _message;
    public:
        explicit Error(std::string msg) : _message(msg) {}
        const char *what() const noexcept override {
            return _message.c_str();
        }
};

class GraphicalError: public Error
{
    public:
        explicit GraphicalError(std::string msg) : Error("Graphical error: " + msg) {}
};

class GameError: public Error
{
    public:
        explicit GameError(std::string msg) : Error("Game error: " + msg) {}
};

class DLLoadError: public Error
{
    public:
        explicit DLLoadError(std::string msg) : Error("DLL Load error: " + msg) {}
};

class CoreError: public Error
{
    public:
        explicit CoreError(std::string msg) : Error("Core error: " + msg) {}
};

#endif
