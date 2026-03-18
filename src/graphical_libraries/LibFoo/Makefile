##
## EPITECH PROJECT, 2026
## bootstrap_arcade
## File description:
## Makefile
##

NAME = libFoo.so

SRC = LibFoo.cpp

CXX = clang++
CXXFLAGS = -Wall -Wextra -fPIC -I../../../include
LDFLAGS = -shared

OUTPUT_DIR = ../../../lib/graphical_lib

all: $(OUTPUT_DIR)/$(NAME)

$(OUTPUT_DIR)/$(NAME): $(SRC) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -f $(OUTPUT_DIR)/$(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re