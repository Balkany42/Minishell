#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

typedef enum e_tokentype
{
    WORD,
    PIPE,
    REDIR_IN,
    REDIR_OUT,
    HEREDOC,
    APPEND,
    INVALID
}   t_tokentype;

typedef enum e_quote {
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
}   t_quote;

typedef struct s_token
{
    char            *value;
    t_tokentype     type;
    t_quote         quote_type;
    struct s_token  *next;
}   t_token;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_redir
{
    t_tokentype     type;   // REDIR_IN, REDIR_OUT, APPEND, HEREDOC
    char            *file;  // nom du fichier ou délimiteur
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char        **args;      // argv
    t_redir     *redirs;     // liste de redirections
    int         fd_in;       // redirection d'entrée
    int         fd_out;      // redirection de sortie
    struct s_cmd *next;      // commande suivante (pipeline)
}   t_cmd;

typedef struct s_minishell
{
    t_env   *env;          // environnement interne
    int     exit_status;   // $? 
    char    **envp;
    int should_exit;
}   t_minishell;

int is_valid_identifier(char *s);
void env_set(t_env **env, char *key, char *value);
int builtin_export(t_cmd *cmd, t_minishell *sh);
int builtin_unset(t_cmd *cmd, t_minishell *sh);
char *env_get(t_env *env, char *key);
char *expand_word(char *word, t_minishell *sh);
char *expand_quotes(char *word, t_minishell *sh);
void expand_args(t_cmd *cmd, t_minishell *sh);
int expand_redirs(t_cmd *cmd, t_minishell *sh);
int expand_cmds(t_cmd *cmds, t_minishell *sh);
t_env *new_env_node(char *key, char *value);
void add_env_back(t_env **env, t_env *new);
t_env *init_env(char **envp);
void executor(t_cmd *cmds, t_minishell *sh);
void exec_single(t_cmd *cmd, t_minishell *sh);
void exec_pipeline(t_cmd *cmds, t_minishell *sh);
int apply_redirs(t_cmd *cmd);
void exec_external(t_cmd *cmd, t_minishell *sh);
char **env_to_tab(t_env *env);
int open_heredoc(char *limiter);
int skip_spaces(char *s, int i);
t_tokentype   is_operator(char *s, int i);
t_token *token_new(char *value, t_tokentype type);
void add_token(t_token **list, char *value, t_tokentype type);
void add_token_with_quote(t_token **list, char *value, t_tokentype type, t_quote q);
void read_word(t_token **list, char *s, int *i);
char *read_single_quote(char *s, int *i);
char *read_double_quote(char *s, int *i);
void free_tokens(t_token *list);
t_token *lexer(char *line);
int		count_args(t_token *list);
t_cmd	*cmd_new(void);
void	add_cmd_to_list(t_cmd **cmd_list, t_cmd *cmd);
void add_arg(t_cmd *cmd, char *arg);
void add_redir(t_cmd *cmd, t_tokentype type, char *file);
t_cmd *parse_command(t_token **list, t_minishell *sh);
t_cmd *parser(t_token *list, t_minishell *sh);
char *expand_token(t_token *tok, t_minishell *sh);
char *readprompt();
void handler_sigint(int sig);
void init_signals(void);
char *ft_strdup(char *src);
int ft_isalpha(int c);
int ft_isalnum(int c);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strchr(const char *s, int c);
char *ft_substr(const char *s, unsigned int start, size_t len);
void sort_env(char **tab);
void print_export_sorted(t_env *env);
size_t ft_strlen(const char *s);
int  count_digits(int n);
char *ft_itoa(int n);
char *ft_strjoin(const char *s1, const char *s2);
int is_builtin(char *cmd);
int exec_builtin(t_cmd *cmd, t_minishell *sh);
int builtin_echo(char **args, t_minishell *sh);
int builtin_pwd(char **args, t_minishell *sh);
int builtin_env(char **args, t_minishell *sh);
int builtin_cd(char **args, t_minishell *sh);
int builtin_exit(char **args, t_minishell *sh);
int ft_isnumber(const char *s);
long ft_atol(const char *s);
char *join_path(const char *dir, const char *cmd);
char *path(char *cmd, char **envp);
void free_tab(char **tab);
void free_cmds(t_cmd *cmds);
char	**ft_split(char const *s, char c);
char	*ft_split2(const char *s, char c, size_t *i);
void	ft_free(char **tab, size_t count);
int	ft_count_words(const char *s, char c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
int check_syntax_errors(t_token *t);
int is_blank_line(char *s);
void    free_env_list(t_env *env);
int is_flag_n(char *s);
char *read_unquoted(char *s, int *i);

#endif