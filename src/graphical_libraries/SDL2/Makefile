##
## EPITECH PROJECT, 2026
## bootstrap_arcade
## File description:
## Makefile
##

NAME = arcade_sdl2.so

SRC = SDL2.cpp

CXX = clang++
CXXFLAGS = -Wall -Wextra -g -fPIC -I../../../include
LDFLAGS = -shared -lSDL2 -lSDL2_ttf -lSDL2_image

OUTPUT_DIR = ../../../lib/graphical_lib

all: $(OUTPUT_DIR)/$(NAME)

$(OUTPUT_DIR)/$(NAME): $(SRC) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -f $(OUTPUT_DIR)/$(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re