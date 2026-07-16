NAME := minishell
CC := cc
CFLAGS := -Wall -Werror -Wextra -g3 -Iinclude

SRC := \
src/001_main.c \
src/002_main_sub.c \
src/003_env_to_tab.c \
src/004_prompt.c \
src/005_signals.c \
src/006_lexer.c \
src/007_lexer_2.c \
src/008_lexer_3.c \
src/009_forbid_tokens.c \
src/010_parser.c \
src/011_parser_2.c \
src/012_parser_3.c \
src/013_expander.c \
src/014_expander_2.c \
src/015_expander_3.c \
src/016_executor.c \
src/017_executor_2.c \
src/018_executor_3.c \
src/019_executor_4.c \
src/020_executor_5.c \
src/021_free.c \
src/022_path.c \
src/023_builtins.c \
src/024_builtins_2.c \
src/025_builtins_3.c \
src/026_builtins_4.c \
src/027_builtins_5.c \
src/028_utils_1.c \
src/029_utils_2.c \
src/030_utils_3.c \
src/031_utils_4.c \
src/032_utils_5.c \
src/033_corrections.c \
src/034_corrections_2.c

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