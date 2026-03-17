/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** libfoo.c
*/


/*
When a dynamic library is loaded and unloaded, it is possible to call a user-defined function to initialize
and finalize the library. To do so, gcc provides the following attributes:
-  __attribute__((constructor))
-  __attribute__((destructor))
The function carrying the constructor attribute is called by dlopen before it returns. The function carrying the
destructor attribute is similarly called by dlclose.
- Create any number of shared libraries using gcc
- Each of these must contain 3 functions:
  -  A procedure carrying the constructor attribute, taking no parameter
  -  A procedure carrying the destructor attribute, taking no parameter
  -  A function with any prototype you like, called myEntryPoint
- Each of these functions must print an appropriate message regarding its role to the standard output
- You must use a loop to load each library, call the myEntryPoint functions, and close each library
- Your main program must not explicitly print out anything!
*/

/*
sample output:
[libfoo] Loading foo library ...
[libbar] Loading bar library ...
[libgra] Loading gra library ...
[libfoo] Entry point for foo !
[libbar] Entry point for bar !
[libgra] Another entry point !
[libfoo] foo closing ...
[libbar] Closing bar ...
[libgra] Gra 's getting out ...
*/

//shared_library.c
#include <stdio.h>

__attribute__((constructor))
void init(void)
{
    printf("[libfoo] Loading foo library...\n");
}

__attribute__((destructor))
void fini(void)
{
    printf("[libfoo] foo closing...\n");
}

int myEntryPoint(void)
{
    printf("[libfoo] Entry point for foo!\n");
    return 0;
}

