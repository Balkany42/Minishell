/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 15:55:54 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 16:08:57 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef enum e_tokentype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	INVALID
}	t_tokentype;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

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
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_minishell
{
	t_env			*env;
	int				exit_status;
	char			**envp;
	int				should_exit;
}	t_minishell;

void		init_minishell(t_minishell *sh, char **envp);
void		run_minishell_loop(t_minishell *sh);
void		process_line(char *line, t_minishell *sh);
int			handle_lexer_and_syntax(char *line,
				t_token **tokens, t_minishell *sh);
void		cleanup_minishell(t_minishell *sh);
char		**env_to_tab(t_env *env);
int			env_count(t_env *env);
char		*env_build_entry(t_env *var);
char		*readprompt(void);
int			is_blank_line(char *s);
void	handler_sigint(int sig);
void	init_signals(void);
t_token		*lexer(char *line);
t_token		*lexer_loop(char *line);
int			handle_operator(t_token **list, char *line, int i,
				t_tokentype type);
int			handle_word(t_token **list, char *line, int i);
int			skip_spaces(char *s, int i);
t_tokentype	is_operator(char *s, int i);
t_token		*token_new(char *value, t_tokentype type);
void		add_token(t_token **list, char *value, t_tokentype type);
void		read_word(t_token **list, char *s, int *i);
void		add_token_with_quote(t_token **list, char *value,
				t_tokentype type, t_quote q);
char		*read_single_quote(char *s, int *i);
char		*read_double_quote(char *s, int *i);
void		free_tokens(t_token *list);
char		*read_unquoted(char *s, int *i);
int			check_syntax_errors(t_token *t);
int			check_first_token(t_token *t);
int			check_invalid_token(t_token *t);
int			check_end_of_line(t_token *t, t_token *n);
int			check_between_tokens(t_token *t, t_token *n);
t_cmd		*parser(t_token *list, t_minishell *sh);
void		add_cmd_to_list(t_cmd **cmd_list, t_cmd *cmd);
t_cmd		*parse_command(t_token **list, t_minishell *sh);
t_token		*parse_word_token(t_cmd *cmd, t_token *tok, t_minishell *sh);
t_token		*parse_redirection_token(t_cmd *cmd, t_token *tok, t_minishell *sh);
int			is_ambiguous_redirect(t_tokentype type, char *expanded);
void		redir_error(t_cmd *cmd, t_token *tok,
				char *expanded, t_minishell *sh);
t_cmd		*cmd_new(void);
void		add_arg(t_cmd *cmd, char *arg);
void		add_redir(t_cmd *cmd, t_tokentype type, char *file);
t_env		*init_env(char **envp);
void		add_env_back(t_env **env, t_env *new);
t_env		*new_env_node(char *key, char *value);
char		*expand_token(t_token *tok, t_minishell *sh);
char		*env_get(t_env *env, char *key);
char		*expand_word(char *word, t_minishell *sh);
char		*process_expansion(char *result,
				char *word, int *i, t_minishell *sh);
char		*handle_escaped_dollar(char *result, int *i);
char		*handle_exit_status(char *result, t_minishell *sh, int *i);
char		*handle_invalid_var(char *result);
char		*handle_backslash(char *result, char *word, int *i);
char		*append_char(char *result, char c);
char		*handle_var_expansion(char *result,
				char *word, int *i, t_minishell *sh);
char		*read_var_name(char *word, int *i);
void		executor(t_cmd *cmds, t_minishell *sh);
void		exec_single(t_cmd *cmd, t_minishell *sh);
void		exec_single_builtin(t_cmd *cmd, t_minishell *sh);
void		exec_single_child(t_cmd *cmd, t_minishell *sh);
void		exec_single_parent(pid_t pid, t_minishell *sh);
void		exec_pipeline(t_cmd *cmds, t_minishell *sh);
void		pipeline_child(t_cmd *cmd,
				t_minishell *sh, int prev_fd, int pipefd[2]);
void		pipeline_parent(t_cmd *cmd, int *prev_fd, int pipefd[2]);
void		pipeline_wait_last(pid_t last_pid, t_minishell *sh);
void		exec_external(t_cmd *cmd, t_minishell *sh);
void		exec_with_path(t_cmd *cmd, t_minishell *sh);
void		exec_from_envpath(t_cmd *cmd, t_minishell *sh);
void		check_is_directory(char *path);
int			apply_redirs(t_cmd *cmd);
int			handle_normal_redir(t_redir *r);
int			open_heredoc(char *limiter);
int			read_heredoc_lines(int write_fd, char *limiter);
void		free_tab(char **tab);
void		free_cmds(t_cmd *cmds);
void		free_redirs(t_redir *r);
void		free_env_list(t_env *env);
char		*path(char *cmd, char **envp);
char		*check_direct_path(char *cmd);
char		*get_path_env(char **envp);
char		*search_in_paths(char **paths, char *cmd);
char		*join_path(const char *dir, const char *cmd);
int			is_builtin(char *cmd);
int			exec_builtin(t_cmd *cmd, t_minishell *sh);
int			handle_special_builtins(char **args, t_minishell *sh);
int			handle_normal_builtins(t_cmd *cmd, t_minishell *sh);
int			builtin_echo(char **args, t_minishell *sh);
int			is_flag_n(char *s);
int			builtin_pwd(char **args, t_minishell *sh);
int			builtin_cd(char **args, t_minishell *sh);
int			builtin_exit(char **args, t_minishell *sh);
int			builtin_env(char **args, t_minishell *sh);
int			builtin_unset(t_cmd *cmd, t_minishell *sh);
void		unset_process_one(char *key, t_minishell *sh);
int			unset_is_invalid(char *key, t_minishell *sh);
void		unset_remove_key(t_env **env, char *key);
int			builtin_export(t_cmd *cmd, t_minishell *sh);
void		print_export_sorted(t_env *env);
void		print_export_entry(char *entry);
int			is_valid_identifier(char *s);
void		export_process_one(char *arg, t_minishell *sh);
char		*export_extract_key(char *arg, char *eq);
void		env_set(t_env **env, char *key, char *value);
int			env_set_update(t_env *env, char *key, char *value);
void		sort_env(char **tab);
char		**ft_split(char const *s, char c);
char		*ft_split_extract(const char *s, char c, size_t *i);
char		*ft_split2(const char *s, char c, size_t *i);
void		ft_free(char **tab, size_t count);
int			ft_count_words(const char *s, char c);
char		*ft_strjoin(const char *s1, const char *s2);
int			ft_isnumber(const char *s);
long		ft_atol(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strdup(char *src);
int			ft_isalpha(int c);
int			ft_isalnum(int c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strchr(const char *s, int c);
char		*ft_substr(const char *s, unsigned int start, size_t len);
char		*ft_substr_copy(const char *s, unsigned int start, size_t len);
char		*ft_itoa(int n);
int			ft_itoa_count_digits(long nb);
void    ft_itoa_fill(char *str, long nb, int len);
size_t		ft_strlen(const char *s);
int			count_digits(int n);

#endif
