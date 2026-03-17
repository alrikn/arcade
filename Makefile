##
## EPITECH PROJECT, 2025
## shared library compiler
## File description:
## Makefile
##we need to compile multiple .so files, wit different source files
##

NAME = core

SRC = main.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=gnu17 -Wall -Wextra -iquote . -g

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

compile: epiclang -std=gnu17 -Wall -Wextra -iquote . main.c -L. -lstring -g

compile_commands.json:
	@bear -- make

.PHONY: all clean fclean re
