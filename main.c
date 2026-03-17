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


int main(void)
{
    for (int i = 0; i < 1; i++) {
        // load the library
        void *handle = dlopen("./libfoo.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Error loading library: %s\n", dlerror());
            return 1;
        }

        // call the myEntryPoint function
        int (*myEntryPoint)(void) = dlsym(handle, "myEntryPoint");
        if (!myEntryPoint) {
            fprintf(stderr, "Error finding symbol: %s\n", dlerror());
            dlclose(handle);
            return 1;
        }
        myEntryPoint();

        // close the library
        dlclose(handle);
    }
    return 0;
}
