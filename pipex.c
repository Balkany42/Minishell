// J'ai balancé tout mon piepex. Il ne renvoie pas les codes d'erreur (on en a 3 à gérer, celui là n'en gère que 2)
// Il va falloir commencer avec ça. Mettre tout en ordre, et déjà commencer par démarer le truc
// Normalement, la plupart des fonctions là ne vont pas servir à grand chose. Parce qu'on doit être capable d'executer avec une seule commande. Là tout ça, ça ne sert que dans le cas ou on a 2 commandes ou plus.
// Pas mal de trucs vont crash là dedans.
// Le pipex pour l'instant marche toujours avec une structure t_pipex. Il va falloir la changer en t_minishell. ça va être assez galère, d'autant que j'ai modifié le pipex en despi, mais je n'ai pas tout revu, tout recompris. J'ai essayé d'intégrer les builtins.
// Spoiler alert, ça n'a pas marché

#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
    t_files f;
    char    ***cmds;
    int     heredoc;
    int     offset;

    heredoc = (argc >= 2 && ft_strcmp(argv[1], "here_doc") == 0);
    offset = heredoc ? 3 : 2;

    cmds = splitcommands_bonus(argc, argv, offset);
    if (!cmds)
        return (ft_printf("Error in command splitting"));

    if (!init_files(&f, argc, argv, heredoc))
        return (free_cmds(cmds), ft_printf("file error"));

    ft_pipex(f.infile, f.outfile, cmds, envp);
    free_cmds(cmds);
    return (0);
}

int init_files(t_files *f, int argc, char **argv, int heredoc)
{
    int hdpipe[2];

    if (heredoc)
    {
        if (pipe(hdpipe) == -1)
            return (0);
        fill_heredoc(argv[2], hdpipe[1]);
        close(hdpipe[1]);
        f->infile = hdpipe[0];
    }
    else
    {
        f->infile = open(argv[1], O_RDONLY);
        if (f->infile < 0)
            perror(argv[1]);
    }
    f->outfile = open(argv[argc - 1],
            O_CREAT | O_WRONLY | (heredoc ? O_APPEND : O_TRUNC), 0644);
    if (f->outfile < 0)
    {
        perror(argv[argc - 1]);
        return (0);
    }
    return (1);
}

#include "pipex.h"

char ***splitcommands(int argc, char **argv, int heredoc)
{
    char ***cmds;
    size_t cmdcount = argc - 3;

    (void) heredoc;
    if (argc < 5)
        return (ft_printf("Argument number below 5\n"), NULL);

    cmds = malloc(sizeof(char **) * (cmdcount + 1));
    if (!cmds)
        return (NULL);

    size_t i = 0;
    size_t arg = 2;

    while (arg < (size_t)(argc - 1))
    {
        cmds[i] = split_with_quotes(argv[arg]);
        if (!cmds[i])
            return (free_cmds(cmds), NULL);
        i++;
        arg++;
    }
    cmds[i] = NULL;
    return (cmds);
}

char ***alloc_cmds(int argc, int heredoc)
{
    char ***cmds;

    cmds = malloc(sizeof(char **) * (argc - heredoc));
    if (!cmds)
        return (NULL);
    return (cmds);
}

int fill_cmds(char ***cmds, int argc, char **argv, int heredoc)
{
    size_t cmd = heredoc;
    size_t tab = 0;

    while (cmd < (size_t)(argc - 1))
    {
        cmds[tab] = split_with_quotes(argv[cmd]);
        if (!cmds[tab])
        {
            free_cmds(cmds);
            return (0);
        }
        cmd++;
        tab++;
    }
    cmds[tab] = NULL;
    return (1);
}

#include "pipex.h"

int ft_pipex(int infile, int outfile, char ***cmds, char **envp)
{
    size_t  cmdcount = 0;
    int     **pipes;
    t_pipex s;

    while (cmds[cmdcount])
        cmdcount++;
    pipes = createpipes(cmdcount);
    if (!pipes)
        return (free_cmds(cmds), 1);
    s.infile = infile;
    s.outfile = outfile;
    s.pipes = pipes;
    s.cmdcount = cmdcount;
    s.cmds = cmds;
    s.envp = envp;
    launch_children(&s);
    close_all_pipes(&s);
    wait_all(&s);
    free_pipes(&s);
    return (0);
}


void free_partial_pipes(int **pipes, size_t count)
{
    size_t i = 0;

    while (i < count)
    {
        free(pipes[i]);
        i++;
    }
    free(pipes);
}

int **createpipes(size_t cmdcount)
{
    int     **pipes;
    size_t  i;

    if (cmdcount < 2)
        return (NULL);
    pipes = malloc(sizeof(int *) * (cmdcount - 1));
    if (!pipes)
        return (NULL);
    i = 0;
    while (i < cmdcount - 1)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i] || pipe(pipes[i]) == -1)
        {
            free_partial_pipes(pipes, i);
            return (NULL);
        }
        i++;
    }
    return (pipes);
}

#include "pipex.h"

void launch_children(t_pipex *structure)
{
    size_t i;
    pid_t pid;

    i = 0;
    while (i < structure->cmdcount)
    {
        pid = fork();
        if (pid == -1)
            return ;
        if (pid == 0)
            childprocess(structure, i);
        i++;
    }
}

void childprocess(t_pipex *s, size_t i)
{
    char *cmdpath;

    if (i == 0)
        setup_first(s);
    else if (i == s->cmdcount - 1)
        setup_last(s, i);
    else
        setup_middle(s, i);
    if (is_builtin(s->cmds[i][0]))
    {
        exec_builtin(s->cmds[i], &s->env); // exécute le builtin
        exit(0); // très important
    }
    if (ft_strchr(s->cmds[i][0], '/'))
    {
        execve(s->cmds[i][0], s->cmds[i], s->envp);
        perror(s->cmds[i][0]);
        exit(127);
    }
    cmdpath = path(s->cmds[i][0], s->envp);
    if (!cmdpath)
    {
        write(2, s->cmds[i][0], ft_strlen(s->cmds[i][0]));
        write(2, ": command not found\n", 21);
        exit(127);
    }
    execve(cmdpath, s->cmds[i], s->envp);
    free(cmdpath);
    perror(s->cmds[i][0]);
    exit(127);
}

void setup_first(t_pipex *s)
{
    if (s->infile < 0)
    {
        int devnull = open("/dev/null", O_RDONLY);
        dup2(devnull, STDIN_FILENO);
        close(devnull);
    }
    else
        dup2(s->infile, STDIN_FILENO);

    dup2(s->pipes[0][1], STDOUT_FILENO);

    close_all_pipes(s);

    if (s->infile >= 0)
        close(s->infile);
}

void setup_middle(t_pipex *s, size_t i)
{
    dup2(s->pipes[i - 1][0], STDIN_FILENO);
    dup2(s->pipes[i][1], STDOUT_FILENO);

    close_all_pipes(s);
}

void setup_last(t_pipex *s, size_t i)
{
    dup2(s->pipes[i - 1][0], STDIN_FILENO);
    dup2(s->outfile, STDOUT_FILENO);

    close_all_pipes(s);

    if (s->outfile > 2)
        close(s->outfile);
}

#include "pipex.h"

char *path(char *cmd, char **envp)
{
    char *env_path;
    char *p;

    env_path = find_path(envp);

    if (ft_strchr(cmd, '/'))
        return ft_strdup(cmd);

    if (!env_path || !*env_path)
    {
        p = ft_pathcmd("/bin", cmd);
        if (access(p, X_OK) == 0)
            return p;
        free(p);
        p = ft_pathcmd("/usr/bin", cmd);
        if (access(p, X_OK) == 0)
            return p;
        free(p);
        return NULL;
    }
    return path_search(env_path, cmd);
}



char *find_path(char **envp)
{
    size_t i;
    i = 0;
    while(envp[i])
    {
        if(ft_strncmp(envp[i], "PATH=", 5) == 0)
            return(envp[i] + 5);
        i++;
    }
    return (NULL);
}

char *ft_pathcmd(char *path, char *cmd)
{
    size_t  i;
    size_t  j;
    char    *test;

    i = 0;
    j = 0;
    test = malloc(ft_strlen(path) + ft_strlen(cmd) + 2);
    if(!test)
        return(NULL);
    while(path[i])
    { 
        test[i] = path[i];
        i++;
    }
    test[i] = '/';
    i++;
    while(cmd[j])
    { 
        test[i] = cmd[j];
        i++;
        j++;
    }
    test[i] = '\0';
    return (test);
}

void free_cmds(char ***cmds)
{
    size_t i = 0;
    size_t j;

    while (cmds[i])
    {
        j = 0;
        while (cmds[i][j])
        {
            free(cmds[i][j]);
            j++;
        }
        free(cmds[i]);
        i++;
    }
    free(cmds);
}

void free_pipes(t_pipex *structure)
{
    size_t i = 0;

    while (i < structure->cmdcount - 1)
    {
        free(structure->pipes[i]);
        i++;
    }
    free(structure->pipes);
}
#include "pipex.h"

void fill_heredoc(char *limiter, int write_fd)
{
    size_t len = ft_strlen(limiter);
    char *line;

    while (1)
    {
        write(1, "heredoc> ", 9);
        line = read_line();
        if (!line)
            return;
        if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
        {
            free(line);
            return;
        }
        write(write_fd, line, ft_strlen(line));
        free(line);
    }
}

char *read_line(void)
{
    char    *buf = malloc(1);
    char    c;
    size_t  len = 0;

    if (!buf)
        return (NULL);
    while (read(STDIN_FILENO, &c, 1) > 0)
    {
        char *tmp = realloc(buf, len + 2);
        if (!tmp)
        {
            free(buf);
            return (NULL);
        }
        buf = tmp;
        buf[len++] = c;
        if (c == '\n')
            break;
    }
    buf[len] = '\0';
    return buf;
}

#include "pipex.h"

int	ft_printf(const char *src, ...)
{
	va_list	args;
	size_t	count;

	count = 0;
	va_start(args, src);
	while (*src)
	{
		if (*src == '%')
		{
			src++;
			count += (ft_args(*src, args));
			src++;
		}
		else
		{
			count += ft_putchar(*src);
			src++;
		}
	}
	va_end(args);
	return (count);
}

int	ft_pointer(void *p)
{
	size_t		count;
	uintptr_t	address;

	address = (uintptr_t)p;
	count = 0;
	if (p == NULL)
	{
		write(1, "(nil)", 5);
		count = 5;
		return (count);
	}
	else
	{
		count += (ft_putstr("0x"));
		count += (ft_putnbr4(address));
		return (count);
	}
}

int	ft_args(const char src, va_list args)
{
	size_t	count;

	count = 0;
	if (src == 'd')
		count += (ft_putnbr(va_arg(args, int)));
	else if (src == '%')
		count += (ft_putchar('%'));
	else if (src == 'c')
		count += (ft_putchar(va_arg(args, int)));
	else if (src == 's')
		count += (ft_putstr(va_arg(args, char *)));
	else if (src == 'p')
		count += ft_pointer(va_arg(args, void *));
	else if (src == 'i')
		count += (ft_putnbr(va_arg(args, unsigned int)));
	else if (src == 'u')
		count += (ft_putnbr5(va_arg(args, unsigned int)));
	else if (src == 'x')
		count += (ft_putnbr3(va_arg(args, unsigned int)));
	else if (src == 'X')
		count += (ft_putnbr2(va_arg(args, unsigned int)));
	return (count);
}

int	ft_putnbr4(uintptr_t n)
{
	uintptr_t	i;
	size_t		count;

	count = 0;
	i = n;
	if (i >= 16)
		count += (ft_putnbr4(i / 16));
	if (i % 16 >= 10)
	{
		ft_putchar(((i % 16) - 10) + 'a');
		count++;
	}
	else
	{
		ft_putchar(i % 16 + '0');
		count++;
	}
	return (count);
}

int	ft_putstr(char *s)
{
	size_t	i;

	if (!s)
	{
		write(1, "(null)", 6);
		return (6);
	}
	i = 0;
	while (s[i])
	{
		ft_putchar(s[i]);
		i++;
	}
	return (i);
}

#include "pipex.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putnbr(int n)
{
	long int	i;
	long int	j;
	int			count;

	count = 0;
	i = n;
	if (i < 0)
	{
		i = -i;
		ft_putchar('-');
		count++;
	}
	if (i >= 10)
		count += (ft_putnbr(i / 10));
	j = i % 10 + 48;
	ft_putchar(j);
	count++;
	return (count);
}

int	ft_putnbr5(unsigned int n)
{
	long int	i;
	long int	j;
	int			count;

	count = 0;
	i = n;
	if (i < 0)
	{
		i = -i;
		ft_putchar('-');
		count++;
	}
	if (i >= 10)
		count += (ft_putnbr5(i / 10));
	j = i % 10 + 48;
	ft_putchar(j);
	count++;
	return (count);
}

int	ft_putnbr2(unsigned int n)
{
	long int	i;
	size_t		count;

	count = 0;
	i = n;
	if (i >= 16)
		count += (ft_putnbr2(i / 16));
	if (i % 16 >= 10)
	{
		ft_putchar(((i % 16) - 10) + 'A');
		count++;
	}
	else
	{
		ft_putchar(i % 16 + '0');
		count++;
	}
	return (count);
}

int	ft_putnbr3(unsigned int n)
{
	long int	i;
	size_t		count;

	count = 0;
	i = n;
	if (i >= 16)
		count += (ft_putnbr3(i / 16));
	if (i % 16 >= 10)
	{
		ft_putchar(((i % 16) - 10) + 'a');
		count++;
	}
	else
	{
		ft_putchar(i % 16 + '0');
		count++;
	}
	return (count);
}

#include "pipex.h"

void free_split(char **tab)
{
    size_t i = 0;

    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}
void close_all_pipes(t_pipex *structure)
{
    size_t i = 0;

    while (i < structure->cmdcount - 1)
    {
        close(structure->pipes[i][0]);
        close(structure->pipes[i][1]);
        i++;
    }
}
void wait_all(t_pipex *structure)
{
    size_t i = 0;

    while (i < structure->cmdcount)
    {
        waitpid(-1, NULL, 0);
        i++;
    }
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *path_search(char *env_path, char *cmd)
{
    char **split;
    char *p;
    size_t i;

    split = ft_split(env_path, ':');
    if (!split)
        return NULL;
    i = 0;
    while (split[i])
    {
        p = ft_pathcmd(split[i], cmd);
        if (access(p, X_OK) == 0)
        {
            free_split(split);
            return p;
        }
        free(p);
        i++;
    }
    free_split(split);
    return NULL;
}

#include "pipex.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (unsigned char)s1[i] == (unsigned char)s2[i])
	{
		if (s1[i] == '\0' || s2[i] == '\0')
			return (0);
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	total_size;
	size_t	i;
	size_t	j;

	if (!s)
		return (NULL);
	j = 0;
	i = ft_strlen(s);
	if (start > i)
		total_size = 0;
	else if (i < (start + len))
		total_size = i - start;
	else
		total_size = len;
	p = (char *)malloc((sizeof (char)) * (total_size + 1));
	if (p == 0)
		return (NULL);
	while (j < total_size)
	{
		p[j] = s[j + start];
		j++;
	}
	p[j] = '\0';
	return (p);
}

char    *ft_strchr(const char *s, int c)
{
    unsigned char   uc;

    uc = (unsigned char)c;
    while (*s)
    {
        if (*s == uc)
            return ((char *)s);
        s++;
    }
    if (uc == '\0')
        return ((char *)s);
    return (NULL);
}

char    *ft_strdup(const char *s)
{
    size_t  len;
    char    *dup;
    size_t  i;

    len = ft_strlen(s);
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

#include "pipex.h"

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	k;
	char	**tab;

	i = 0;
	k = 0;
	tab = (char **)malloc(sizeof(char *) * ((ft_count_words(s, c) + 1)));
	if (tab == NULL || s == NULL)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		tab[k] = ft_split2 (s, c, &i);
		if (tab[k] == NULL)
		{
			ft_free(tab, k);
			return (NULL);
		}
		k++;
	}
	tab[k] = (NULL);
	return (tab);
}

char	*ft_split2(const char *s, char c, size_t *i)
{
	size_t	j;
	size_t	start;

	j = 0;
	while (s[*i] == c)
		(*i)++;
	start = *i;
	while (s[*i] != c && s[*i] != '\0')
	{
		j++;
		(*i)++;
	}
	return (ft_substr(s, start, j));
}

void	ft_free(char **tab, size_t count)
{
	while (count > 0)
	{
		count--;
		free(tab[count]);
	}
	free(tab);
}

int	ft_count_words(const char *s, char c)
{
	size_t	i;
	size_t	word_nbr;
	size_t	content;

	i = 0;
	word_nbr = 0;
	content = 0;
	while (s[i])
	{
		if (s[i] != c && content == 0)
		{
			content = 1;
			word_nbr++;
		}
		else if (s[i] == c)
		{
			content = 0;
		}
		i++;
	}
	return (word_nbr);
}

#include "pipex.h"

char ***splitcommands_bonus(int argc, char **argv, int offset)
{
    char    ***cmds;
    size_t  tab = 0;
    size_t  i = offset;
    size_t max_cmds = argc - offset - 1;

    cmds = malloc(sizeof(char **) * (max_cmds + 1));
    if (!cmds)
        return (NULL);

    while (i < (size_t)(argc - 1))
    {
        tab = add_and_commands(cmds, argv[i], tab);
        if (tab == (size_t)-1)
            return (free_cmds(cmds), NULL);
        i++;
    }
    cmds[tab] = NULL;
    return (cmds);
}

size_t add_and_commands(char ***cmds, char *arg, size_t tab)
{
    char    **ands;
    size_t  j;

    ands = split_and(arg);
    if (!ands)
        return ((size_t)-1);
    j = 0;
    while (ands[j])
    {
        cmds[tab] = split_with_quotes(ands[j]);
        if (!cmds[tab])
        {
            free_split(ands);
            return ((size_t)-1);
        }
        tab++;
        j++;
    }
    free_split(ands);
    return (tab);
}

char **split_and(char *cmd)
{
    char    **res;
    size_t  parts = count_and(cmd);
    size_t  i = 0;
    size_t  j = 0;
    size_t  start;

    res = malloc(sizeof(char *) * (parts + 1));
    if (!res)
        return (NULL);
    while (cmd[i])
    {
        start = i;
        while (cmd[i] && !(cmd[i] == '&' && cmd[i + 1] == '&'))
            i++;
        res[j] = ft_substr(cmd, start, i - start);
        if (!res[j])
            return (free_split(res), NULL);
        j++;
        if (cmd[i] == '&' && cmd[i + 1] == '&')
            i += 2;
    }
    res[j] = NULL;
    return (res);
}

size_t count_and(char *cmd)
{
    size_t i = 0;
    size_t count = 1;

    while (cmd[i])
    {
        if (cmd[i] == '&' && cmd[i + 1] == '&')
            count++;
        i++;
    }
    return (count);
}

// A partir d'ici, normalement, on a une partie qui est déjà
// Dans le lexer. J'ai déjà géré pas mal de trucs liés aux quotes

#include "pipex.h"

char **split_with_quotes(const char *s)
{
    t_node  *head;
    char    *arg;
    size_t  count;

    head = NULL;
    count = 0;
    while (*s)
    {
        arg = next_arg(&s);
        if (!arg)
            break;
        if (!push_arg(&head, arg))
            return (NULL);
        count++;
    }
    return (list_to_array(head, count));
}

char **list_to_array(t_node *head, size_t count)
{
    char    **out;
    t_node  *tmp;
    size_t  i;

    out = malloc(sizeof(char *) * (count + 1));
    if (!out)
        return (NULL);
    i = count;
    while (i--)
    {
        out[i] = head->arg;
        tmp = head->next;
        free(head);
        head = tmp;
    }
    out[count] = NULL;
    return (out);
}

int push_arg(t_node **head, char *arg)
{
    t_node  *n;

    n = malloc(sizeof(t_node));
    if (!n)
        return (0);
    n->arg = arg;
    n->next = *head;
    *head = n;
    return (1);
}

char *next_arg(const char **s)
{
    skip_spaces(s);
    if (**s == '\0')
        return (NULL);
    if (**s == '"' || **s == '\'')
        return (extract_quoted(s));
    return (extract_unquoted(s));
}

char *extract_unquoted(const char **s)
{
    const char  *start;
    size_t      len;

    start = *s;
    len = 0;
    while (**s && **s != ' ')
    {
        (*s)++;
        len++;
    }
    return (ft_substr(start, 0, len));
}

char *extract_quoted(const char **s)
{
    char        quote;
    const char  *start;
    size_t      len;

    quote = **s;
    (*s)++;
    start = *s;
    len = 0;
    while (**s && **s != quote)
    {
        (*s)++;
        len++;
    }
    if (**s == quote)
        (*s)++;
    return (ft_substr(start, 0, len));
}

void skip_spaces(const char **s)
{
    while (**s == ' ')
        (*s)++;
}