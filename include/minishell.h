#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_tokentype
{
    WORD,
    PIPE,
    REDIR_IN,
    REDIR_OUT,
    HEREDOC,
    APPEND
}   t_tokentype;

typedef struct s_token
{
    char            *value;
    t_tokentype     type;
    struct s_token  *next;
}   t_token;

#endif