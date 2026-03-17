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
#include "IDisplayModule.hpp"
#include "DLLoader.hpp"
#include "LibFoo.hpp"


int main(void)
{
    DLLoader<IDisplayModule> loader("./libfoo.so");

    IDisplayModule* module = loader.getInstance();

    printf("Name: %s\n", module->getName().c_str());


    return 0;
}
