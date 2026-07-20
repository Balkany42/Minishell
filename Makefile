NAME := minishell
NAME_BONUS := minishell_bonus
CC := cc
CFLAGS := -Wall -Werror -Wextra -g3

SRC := \
src/001_init/001_main.c \
src/001_init/002_main_sub.c \
src/001_init/003_init_env.c \
src/001_init/004_init_prompt.c \
src/001_init/005_init_signals.c \
src/002_lexer/001_lexer.c \
src/002_lexer/002_lexer_2.c \
src/002_lexer/003_lexer_3.c \
src/003_parser/001_forbid_tokens.c \
src/003_parser/002_parser.c \
src/003_parser/003_parser_commands.c \
src/003_parser/004_parser_args.c \
src/004_expander/001_expander.c \
src/004_expander/002_expander_2.c \
src/004_expander/003_expander_3.c \
src/005_exec/001_executor.c \
src/005_exec/002_executor_2.c \
src/005_exec/003_executor_3.c \
src/005_exec/004_executor_4.c \
src/005_exec/005_executor_5.c \
src/005_exec/006_path.c \
src/005_exec/007_heredoc_1.c \
src/005_exec/008_heredoc_2.c \
src/005_exec/009_builtins.c \
src/005_exec/010_builtins_2.c \
src/005_exec/011_builtins_3.c \
src/005_exec/012_builtins_4.c \
src/005_exec/013_builtins_5.c \
src/006_utils/001_free.c \
src/006_utils/002_utils_1.c \
src/006_utils/003_utils_2.c \
src/006_utils/004_utils_3.c \
src/006_utils/005_utils_4.c \
src/006_utils/006_utils_5.c

SRC_BONUS := \
src_bonus/001_init/001_main.c \
src_bonus/001_init/002_main_sub.c \
src_bonus/001_init/003_init_env.c \
src_bonus/001_init/004_init_prompt.c \
src_bonus/001_init/005_init_signals.c \
src_bonus/002_lexer/001_lexer.c \
src_bonus/002_lexer/002_lexer_2.c \
src_bonus/002_lexer/003_lexer_3.c \
src_bonus/003_parser/001_forbid_tokens.c \
src_bonus/003_parser/002_forbid_tokens_2.c \
src_bonus/003_parser/003_parser.c \
src_bonus/003_parser/004_parser_2.c \
src_bonus/003_parser/005_parser_3.c \
src_bonus/004_expander/001_expander.c \
src_bonus/004_expander/002_expander_2.c \
src_bonus/004_expander/003_expander_3.c \
src_bonus/005_exec/001_executor.c \
src_bonus/005_exec/002_executor_2.c \
src_bonus/005_exec/003_executor_3.c \
src_bonus/005_exec/004_executor_4.c \
src_bonus/005_exec/005_executor_5.c \
src_bonus/005_exec/006_path.c \
src_bonus/005_exec/007_heredoc_1.c \
src_bonus/005_exec/008_heredoc_2.c \
src_bonus/005_exec/009_builtins.c \
src_bonus/005_exec/010_builtins_2.c \
src_bonus/005_exec/011_builtins_3.c \
src_bonus/005_exec/012_builtins_4.c \
src_bonus/005_exec/013_builtins_5.c \
src_bonus/006_bonus/001_bonus_wildcard_1.c \
src_bonus/006_bonus/002_bonus_wildcard_2.c \
src_bonus/006_bonus/003_bonus_wildcard_3.c \
src_bonus/006_bonus/004_bonus_ast_1.c \
src_bonus/006_bonus/005_bonus_ast_2.c \
src_bonus/006_bonus/006_bonus_ast_3.c \
src_bonus/006_bonus/007_utils_ast.c \
src_bonus/007_utils/001_free.c \
src_bonus/007_utils/002_utils_1.c \
src_bonus/007_utils/003_utils_2.c \
src_bonus/007_utils/004_utils_3.c \
src_bonus/007_utils/005_utils_4.c \
src_bonus/007_utils/006_utils_5.c

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