#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <sys/wait.h>

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

char *path(char *cmd, char **envp);
char *ft_pathcmd(char *path, char *cmd);
char *find_path(char **envp);
void free_cmds(char ***cmds);
void free_pipes(t_pipex *structure);
int ft_pipex(int infile, int outfile, char ***cmds, char **envp);
char ***splitcommands(int argc, char **argv, int heredoc);
int **createpipes (size_t cmdcount);
void launch_children(t_pipex *structure);
void childprocess(t_pipex *structure, size_t i);
char	**ft_split(char const *s, char c);
char	*ft_split2(const char *s, char c, size_t *i);
void	ft_free(char **tab, size_t count);
int	ft_count_words(const char *s, char c);
void free_split(char **tab);
void close_all_pipes(t_pipex *structure);
void wait_all(t_pipex *structure);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int	ft_printf(const char *src, ...);
int	ft_pointer(void *p);
int	ft_args(const char src, va_list args);
int	ft_putnbr4(uintptr_t n);
int	ft_putstr(char *s);
int	ft_putchar(char c);
int	ft_putnbr(int n);
int	ft_putnbr5(unsigned int n);
int	ft_putnbr2(unsigned int n);
int	ft_putnbr3(unsigned int n);
void    fill_heredoc(char *limiter, int write_fd);
int	ft_strcmp(const char *s1, const char *s2);
char *read_line(void);
char ***alloc_cmds(int argc, int heredoc);
int fill_cmds(char ***cmds, int argc, char **argv, int heredoc);
void free_partial_pipes(int **pipes, size_t count);
void setup_first(t_pipex *s);
void setup_middle(t_pipex *s, size_t i);
void setup_last(t_pipex *s, size_t i);
int init_files(t_files *f, int argc, char **argv, int heredoc);
size_t add_and_commands(char ***cmds, char *arg, size_t tab);
char ***splitcommands_bonus(int argc, char **argv, int offset);
char **split_and(char *cmd);
size_t count_and(char *cmd);
void close_all_pipes_except(t_pipex *s, int fd1, int fd2);
char    *ft_strchr(const char *s, int c);
char **split_with_quotes(const char *s);
char **list_to_array(t_node *head, size_t count);
int push_arg(t_node **head, char *arg);
char *next_arg(const char **s);
char *extract_unquoted(const char **s);
char *extract_quoted(const char **s);
void skip_spaces(const char **s);
char    *ft_strdup(const char *s);
char *path_search(char *env_path, char *cmd);

#endif