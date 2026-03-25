##
## EPITECH PROJECT, 2025
## shared library compiler
## File description:
## Makefile
##
##

NAME = arcade

SRC = main.cpp \
	src/game_libraries/menu_game/MenuGame.cpp \
	src/Core.cpp
OBJ = $(SRC:.cpp=.o)

GRAPHICAL_DIRS = \
	src/graphical_libraries/Ncurses \
	src/graphical_libraries/SFML \

GAME_DIRS = \
	src/game_libraries/snake_game \
	src/game_libraries/nibbler \

SHARED_FLAGS = -shared -fPIC


#where we put to all the libraries
LIB_DIR = lib/game_lib lib/graphical_lib

CC = clang++
CFLAGS = -Wall -Wextra -g -Iinclude

all: $(NAME) libs

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

graphical:
	@for dir in $(GRAPHICAL_DIRS); do \
		$(MAKE) -C $$dir; \
	done

#libs needs to execute graphical
libs:
	@for dir in $(GRAPHICAL_DIRS); do \
		$(MAKE) -C $$dir; \
	done
	@for dir in $(GAME_DIRS); do \
		$(MAKE) -C $$dir; \
	done

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -f $(OBJ)
	@for dir in $(GRAPHICAL_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	@for dir in $(GAME_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

fclean: clean
	rm -f $(NAME)
	@for dir in $(GRAPHICAL_DIRS); do \
		$(MAKE) -C $$dir fclean; \
	done
	@for dir in $(GAME_DIRS); do \
		$(MAKE) -C $$dir fclean; \
	done

re: fclean all



compile_commands.json:
	@bear -- make

.PHONY: all clean fclean re compile_main libs
