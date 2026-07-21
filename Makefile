# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/20 16:55:12 by megrelli          #+#    #+#              #
#    Updated: 2026/07/20 18:13:45 by megrelli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
BONUS_NAME  = minishell_bonus

CFLAGS  = -Wall -Werror -Wextra -Iinclude -Iinclude_bonus -g3
Libft   = ./libft/libft.a
OBJ_DIR = objs

# --- COLORS ---
GREEN	= \033[0;32m
CYAN	= \033[1;36m
RED		= \033[0;31m
BLUE	= \033[0;94m
RESET	= \033[0m

# --- SRCS ---
SRCS = src/init/main.c src/init/main_sub.c \
src/init/init_env.c src/init/init_prompt.c \
src/init/init_signals.c src/lexer/lexer.c \
src/lexer/lexer_2.c src/lexer/lexer_3.c \
src/parser/forbid_tokens.c src/parser/parser.c \
src/parser/parser_commands.c src/parser/parser_args.c \
src/expander/expander.c src/expander/expander_2.c \
src/expander/expander_3.c src/exec/executor.c \
src/exec/executor_2.c src/exec/executor_3.c \
src/exec/executor_4.c src/exec/executor_5.c \
src/exec/path.c src/exec/heredoc_1.c \
src/exec/heredoc_2.c src/builtins/builtins.c \
src/builtins/builtins_2.c src/builtins/builtins_3.c \
src/builtins/builtins_4.c src/builtins/builtins_5.c \
src/utils/free.c src/utils/utils_5.c

BONUS_SRCS = src_bonus/init/main.c src_bonus/init/main_sub.c \
src_bonus/init/init_env.c src_bonus/init/init_prompt.c \
src_bonus/init/init_signals.c src_bonus/lexer/lexer.c \
src_bonus/lexer/lexer_2.c src_bonus/lexer/lexer_3.c \
src_bonus/parser/forbid_tokens.c \
src_bonus/parser/forbid_tokens_2.c src_bonus/parser/parser.c \
src_bonus/parser/parser_2.c src_bonus/parser/parser_3.c \
src_bonus/expander/expander.c src_bonus/expander/expander_2.c \
src_bonus/expander/expander_3.c src_bonus/exec/executor.c \
src_bonus/exec/executor_2.c src_bonus/exec/executor_3.c \
src_bonus/exec/executor_4.c src_bonus/exec/executor_5.c \
src_bonus/exec/path.c src_bonus/exec/heredoc_1.c \
src_bonus/exec/heredoc_2.c src_bonus/builtins/builtins.c \
src_bonus/builtins/builtins_2.c src_bonus/builtins/builtins_3.c \
src_bonus/builtins/builtins_4.c src_bonus/builtins/builtins_5.c \
src_bonus/bonus/bonus_wildcard_1.c \
src_bonus/bonus/bonus_wildcard_2.c \
src_bonus/bonus/bonus_wildcard_3.c \
src_bonus/bonus/bonus_ast_1.c src_bonus/bonus/bonus_ast_2.c \
src_bonus/bonus/bonus_ast_3.c src_bonus/bonus/utils_ast.c \
src_bonus/utils/free.c src_bonus/utils/utils_5.c

# --- OBJS ---
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
BONUS_OBJS = $(BONUS_SRCS:%.c=$(OBJ_DIR)/%.o)

# --- RULES ---
all: $(NAME)

$(Libft):
	@printf "$(CYAN)┌── $(BLUE)Compiling Libft...$(RESET)\n"
	@make -C ./libft > /dev/null
	@printf "$(CYAN)└── $(BLUE)Libft OK$(RESET)\n"

$(NAME): $(OBJS) $(Libft)
	@printf "\n$(GREEN)Building $(NAME)...$(RESET)\n"
	@cc $(CFLAGS) $(OBJS) $(Libft) -o $(NAME) -lreadline -lhistory
	@printf "$(GREEN)Done! $(NAME) is ready $(RESET)\n"

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(Libft)
	@printf "\n$(GREEN)Building $(BONUS_NAME)...$(RESET)\n"
	@cc $(CFLAGS) $(BONUS_OBJS) $(Libft) -o $(BONUS_NAME) -lreadline -lhistory
	@printf "$(GREEN)Done! $(BONUS_NAME) is ready $(RESET)\n"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(CYAN)│ $(BLUE) Compiling $<...$(RESET)\n"
	@cc $(CFLAGS) -c $< -o $@

clean:
	@printf "$(RED)Cleaning objects...$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@make -C ./libft clean > /dev/null

fclean: clean
	@printf "$(RED)Cleaning executables...$(RESET)\n"
	@rm -f $(NAME) $(BONUS_NAME)
	@make -C ./libft fclean > /dev/null

re: fclean all

ree: fclean bonus

.PHONY: all bonus clean fclean re ree
