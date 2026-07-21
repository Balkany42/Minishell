# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/12 17:29:50 by megrelli          #+#    #+#              #
#    Updated: 2026/07/15 04:53:34 by megrelli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Werror -Wextra -I.

SRC = ft_atoi.c ft_bzero.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
ft_isprint.c ft_isalnum.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strdup.c \
ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_tolower.c \
ft_toupper.c ft_calloc.c ft_strchr.c ft_strrchr.c ft_strtrim.c \
ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_putnbr_fd.c ft_memchr.c \
ft_substr.c ft_strjoin.c ft_memcmp.c ft_strmapi.c ft_striteri.c \
ft_split.c ft_itoa.c get_next_line.c ft_printf/ft_printf.c \
ft_printf/ft_printf_utils.c ft_printf/ft_printhexa.c ft_printf/ft_printpointer.c \
ft_printf/ft_printnbr_uns.c ft_free.c ft_atol.c ft_strcmp.c ft_isnumber.c \

BONUS_SRC = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c \
ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c \

OBJ = $(SRC:.c=.o)

BONUS_OBJ = $(BONUS_SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

bonus: $(BONUS_OBJ) $(OBJ)
	ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re 