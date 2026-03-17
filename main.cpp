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


int main(void)
{

    void *handle = dlopen("./libfoo.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error loading library: %s\n", dlerror());
        return 1;
    }

    IDisplayModule* (*create)();
    void (*destroy)(IDisplayModule*);

    create = (IDisplayModule* (*)())dlsym(handle, "create");
    destroy = (void (*)(IDisplayModule*))dlsym(handle, "destroy");

    IDisplayModule* module = create();

    //clang main.cpp -o core
    ///usr/bin/ld: /tmp/main-7bfc53.o: in function `main':
    //main.cpp:(.text+0xa0): undefined reference to `std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::c_str() const'
    //clang: error: linker command failed with exit code 1 (use -v to see invocation)
    //we can fix this by linking the C++ standard library, which is needed for std::string
    //clang main.cpp -o core -lstdc++
    printf("Name: %s\n", module->getName().c_str());

    destroy(module);
    dlclose(handle);
    return 0;
}
