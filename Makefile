NAME := minishell
CC := cc
CFLAGS := -Wall -Werror -Wextra -g3 -Iinclude

SRC := \
src/builtins.c \
src/env_setup.c \
src/env.c \
src/executor.c \
src/lexer_utils.c \
src/lexer.c \
src/main.c \
src/parser.c \
src/prompt.c \
src/signals.c \
src/utils.c \
src/path.c \
src/free.c \
src/split.c \
src/forbid_tokens.c

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lreadline -lhistory -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus