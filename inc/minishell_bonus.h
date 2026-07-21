/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 15:55:54 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 00:02:58 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/stat.h>
# include <dirent.h>
# include "../libft/libft.h"

typedef enum e_tokentype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	INVALID,
	LPAREN,
	RPAREN,
	AND_IF,
	OR_IF,
}	t_tokentype;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

typedef enum e_ast_op
{
	AST_NONE,
	AST_AND_IF,
	AST_OR_IF,
	AST_GROUP
}	t_ast_op;

typedef struct s_token
{
	char			*value;
	t_tokentype		type;
	t_quote			quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redir
{
	t_tokentype		type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				fd_in;
	int				fd_out;
	struct s_ast	*group_ast;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_minishell
{
	t_env			*env;
	int				exit_status;
	char			**envp;
	int				should_exit;
	int				redir_error;
	int				heredoc_interrupted;
}	t_minishell;

typedef struct s_pipectx
{
	t_minishell		*sh;
	int				*prev_fd;
	int				pipefd[2];
	pid_t			*last_pid;
}	t_pipectx;

typedef struct s_wildlist
{
	char				*name;
	struct s_wildlist	*next;
}	t_wildlist;

typedef struct s_ast
{
	t_ast_op		op;
	struct s_ast	*left;
	struct s_ast	*right;
	t_cmd			*cmd;
}	t_ast;

void		init_minishell(t_minishell *sh, char **envp);
void		run_minishell_loop(t_minishell *sh);
void		cleanup_minishell(t_minishell *sh);
char		**env_to_tab(t_env *env);
char		*readprompt(void);
int			is_blank_line(char *s);
void		handler_sigint(int sig);
void		init_signals(void);
t_token		*lexer(char *line);
int			skip_spaces(char *s, int i);
t_tokentype	is_operator(char *s, int i);
int			read_word(t_token **list, char *s, int *i);
void		add_token_with_quote(t_token **list, char *value,
				t_tokentype type, t_quote q);
char		*read_single_quote(char *s, int *i);
char		*read_double_quote(char *s, int *i);
void		free_tokens(t_token *list);
char		*read_unquoted(char *s, int *i);
int			check_syntax_errors(t_token *t);
int			check_pipe_and_redir_relations(t_token *t, t_token *n);
int			check_logic_and_paren_relations(t_token *t, t_token *n);
t_cmd		*parser(t_token *list, t_minishell *sh);
void		add_cmd_to_list(t_cmd **cmd_list, t_cmd *cmd);
t_cmd		*parse_command(t_token **list, t_minishell *sh);
t_token		*parse_redirection_token(t_cmd *cmd, t_token *tok, t_minishell *sh);
t_cmd		*cmd_new(void);
void		add_arg(t_cmd *cmd, char *arg);
void		add_redir(t_cmd *cmd, t_tokentype type, char *file);
t_env		*init_env(char **envp);
char		*expand_token(t_token *tok, t_minishell *sh);
char		*env_get(t_env *env, char *key);
char		*expand_word(char *word, t_minishell *sh);
char		*handle_backslash(char *result, char *word, int *i);
char		*append_char(char *result, char c);
char		*handle_var_expansion(char *result,
				char *word, int *i, t_minishell *sh);
void		executor(t_cmd *cmds, t_minishell *sh);
int			exec_pipeline(t_cmd *cmds, t_minishell *sh);
void		pipeline_child(t_cmd *cmd,
				t_minishell *sh, int prev_fd, int pipefd[2]);
void		pipeline_parent(t_cmd *cmd, int *prev_fd, int pipefd[2]);
void		pipeline_wait_last(pid_t last_pid, t_minishell *sh);
void		exec_external(t_cmd *cmd, t_minishell *sh);
int			apply_redirs(t_cmd *cmd);
void		free_tab(char **tab);
void		free_cmds(t_cmd *cmds);
void		free_redirs(t_redir *r);
void		free_env_list(t_env *env);
char		*path(char *cmd, char **envp);
int			is_builtin(char *cmd);
int			exec_builtin(t_cmd *cmd, t_minishell *sh);
int			builtin_echo(char **args, t_minishell *sh);
int			builtin_pwd(char **args, t_minishell *sh);
int			builtin_cd(char **args, t_minishell *sh);
int			builtin_exit(char **args, t_minishell *sh);
int			builtin_env(char **args, t_minishell *sh);
int			builtin_unset(t_cmd *cmd, t_minishell *sh);
int			builtin_export(t_cmd *cmd, t_minishell *sh);
int			is_valid_identifier(char *s);
void		export_process_one(char *arg, t_minishell *sh);
void		sort_env(char **tab);
int			count_digits(int n);
int			write_heredoc_to(const char *path, const char *limiter);
char		*generate_heredoc_tmp_name(void);
char		*str_join_and_free(char *a, char *b);
void		heredoc_sigint(int sig);
int			has_unquoted_wildcard(char *s);
char		*expand_wildcard(char *pattern);
char		*expand_collect_matches(char *pattern, t_wildlist *files,
				int *found);
char		*expand_sort_result(char *result);
int			wild_match(char *pattern, char *name, int i, int j);
void		free_split(char **arr);
size_t		ft_strlen_2(const char *s);
t_ast		*parse_command_ast(t_token **t, t_minishell *sh);
t_ast		*parse_group_ast(t_token **t, t_minishell *sh);
t_ast		*parse_pipeline(t_token **t, t_minishell *sh);
t_ast		*parse_and_or(t_token **t, t_minishell *sh);
int			exec_ast(t_ast *node, t_minishell *sh);
t_cmd		*parse_pipeline_segment(t_token **list, t_minishell *sh);
void		free_ast(t_ast *node);
void		prepare_heredocs_ast(t_ast *node, t_minishell *sh);
int			check_parentheses_balance(t_token *t);
int			check_parentheses_order(t_token *t);
char		*ast_to_string(t_ast *node);
char		*cmd_to_string(t_cmd *cmd);
int			is_redir(int type);
char		*ft_strjoin3(const char *a, const char *b, const char *c);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		bubble_sort(char **tab);
t_cmd		*cmd_from_group(t_ast *group);
void		exec_group_child(t_cmd *cmd, t_minishell *sh, int prev_fd,
				int pipefd[2]);
void		append_cmd_list(t_cmd *list, t_cmd *to_append);

#endif
