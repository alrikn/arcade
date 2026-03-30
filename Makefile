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
	src/game_libraries/tetris_game \
	src/game_libraries/nibbler \



#where we put to all the libraries
LIB_DIR = lib lib/game_lib lib/graphical_lib

CC = clang++
CFLAGS = -Wall -Wextra -g -Iinclude

all: $(NAME) libs

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

graphical: $(LIB_DIR)
	@for dir in $(GRAPHICAL_DIRS); do \
		$(MAKE) -C $$dir; \
	done
	@cp lib/graphical_lib/*.so lib/ 2>/dev/null || true

game: $(LIB_DIR)
	@for dir in $(GAME_DIRS); do \
		$(MAKE) -C $$dir; \
	done
	@cp lib/game_lib/*.so lib/ 2>/dev/null || true

#libs needs to execute graphical
libs: $(LIB_DIR) graphical game
	@echo "All libraries compiled and copied to lib/ root."


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
	rm -f lib/*.so

re: fclean all



compile_commands.json:
	@bear -- make

.PHONY: all clean fclean re compile_main libs
