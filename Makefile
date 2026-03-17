##
## EPITECH PROJECT, 2025
## shared library compiler
## File description:
## Makefile
##

NAME = core

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

GRAPHICAL_SRC = \
	src/graphical_libraries/LibFoo/LibFoo.cpp \
	src/graphical_libraries/Ncurses/Ncurses.cpp

SHARED_FLAGS = -shared -fPIC

GRAPHICAL_LIBS = $(GRAPHICAL_SRC:.cpp=.so)

#where we put to .so objects
LIB_DIR = lib/graphical_lib

CC = clang++
CFLAGS = -Wall -Wextra -g -Iinclude

all: $(NAME) libs

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.so: %.cpp
	$(CC) $(CFLAGS) $(SHARED_FLAGS) -Iinclude -o $@ $<

libs: $(GRAPHICAL_LIBS)


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -f $(GRAPHICAL_LIBS)

re: fclean all



compile_commands.json:
	@bear -- make

.PHONY: all clean fclean re compile_main libs
