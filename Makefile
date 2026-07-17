NAME := minishell
NAME_BONUS := minishell_bonus
CC := cc
CFLAGS := -Wall -Werror -Wextra -g3

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

SRC_BONUS := \
src_bonus/001_main.c \
src_bonus/002_main_sub.c \
src_bonus/003_env_to_tab.c \
src_bonus/004_prompt.c \
src_bonus/005_signals.c \
src_bonus/006_lexer.c \
src_bonus/007_lexer_2.c \
src_bonus/008_lexer_3.c \
src_bonus/009_forbid_tokens.c \
src_bonus/010_parser.c \
src_bonus/011_parser_2.c \
src_bonus/012_parser_3.c \
src_bonus/013_expander.c \
src_bonus/014_expander_2.c \
src_bonus/015_expander_3.c \
src_bonus/016_executor.c \
src_bonus/017_executor_2.c \
src_bonus/018_executor_3.c \
src_bonus/019_executor_4.c \
src_bonus/020_executor_5.c \
src_bonus/021_free.c \
src_bonus/022_path.c \
src_bonus/023_builtins.c \
src_bonus/024_builtins_2.c \
src_bonus/025_builtins_3.c \
src_bonus/026_builtins_4.c \
src_bonus/027_builtins_5.c \
src_bonus/028_utils_1.c \
src_bonus/029_utils_2.c \
src_bonus/030_utils_3.c \
src_bonus/031_utils_4.c \
src_bonus/032_utils_5.c \
src_bonus/033_corrections.c \
src_bonus/034_corrections_2.c \
src_bonus/bonus_wildcards.c

OBJ := $(SRC:.c=.o)
OBJ_BONUS := $(SRC_BONUS:.c=.o)

all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -Iinclude $(OBJ) -lreadline -lhistory -o $(NAME)

$(NAME_BONUS): $(OBJ_BONUS)
	$(CC) $(CFLAGS) -Iinclude_bonus $(OBJ_BONUS) -lreadline -lhistory -o $(NAME_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re bonus