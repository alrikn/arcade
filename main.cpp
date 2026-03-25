/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** main
** this will call the shared objects.
** a loop to load each library, call the myEntryPoint functions, and close each library
*/

#include <dlfcn.h>
#include <stdio.h>
#include "Core.hpp"
#include "IDisplayModule.hpp"
#include "DLLoader.hpp"
#include "IGameModule.hpp"
#include "MenuGame.hpp"



int main(void)
{
    Core core;
    core.run();

    return 0;
}
