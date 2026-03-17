##
## EPITECH PROJECT, 2025
## shared library compiler
## File description:
## Makefile
## we only compile the main here, the shared library is compiled separately and loaded at runtime using dlopen and dlsym
##

NAME = core

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

CC = clang++
CFLAGS = -Wall -Wextra -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all



compile_commands.json:
	@bear -- make

.PHONY: all clean fclean re compile_main
