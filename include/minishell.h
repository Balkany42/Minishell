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

typedef struct s_files
{
    int   infile;
    int   outfile;
    char *infile_path;
    char *outfile_path;
}   t_files;

typedef struct s_node
{
    char            *arg;
    struct s_node   *next;
}   t_node;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_pipex
{
    int     infile;
    int     outfile;
    char *infile_path;
    char *outfile_path;
    int   **pipes;
    size_t  cmdcount;
    char  ***cmds;
    char   **envp;
}   t_pipex;

#endif