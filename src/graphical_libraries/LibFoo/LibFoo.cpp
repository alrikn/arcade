/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** libfoo.c
*/

#include <stdio.h>
#include "LibFoo.hpp"

void LibFoo::init(void)
{
    printf("[libfoo] init\n");
}

void LibFoo::stop(void)
{
    printf("[libfoo] stop\n");
}

const std::string &LibFoo::getName() const
{
    return _name;
}

//for load/unload side effects (optional)
__attribute__((constructor))
static void onLoad(void)
{
    printf("[libfoo] Library loaded\n");
}

__attribute__((destructor))
static void onUnload(void)
{
    printf("[libfoo] Library unloaded\n");
}

//C interface (THIS is what dlopen/dlsym uses)
extern "C" {

IDisplayModule* create()
{
    return new LibFoo();
}

void destroy(IDisplayModule* instance)
{
    delete instance;
}

}

void LibFoo::drawSprite(const Sprite &sprite, int x, int y)
{
}
