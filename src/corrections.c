#include "../include/minishell.h"

void    prepare_heredocs(t_cmd *cmds, t_minishell *sh)
{
    t_cmd   *cmd;
    t_redir *r;
    char    *tmp;

    cmd = cmds;
    while (cmd)
    {
        r = cmd->redirs;
        while (r)
        {
            if (r->type == HEREDOC)
            {
                // 1) créer un fichier temporaire
                tmp = generate_heredoc_tmp_name();
                if (!tmp)
                {
                    sh->exit_status = 1;
                    return;
                }

                // 2) lire le heredoc et écrire dedans
                if (write_heredoc_to(tmp, r->file) < 0)
                {
                    sh->exit_status = 1;
                    free(tmp);
                    return;
                }

                // 3) remplacer le limiter par le fichier temporaire
                free(r->file);
                r->file = tmp;

                // 4) transformer HEREDOC → REDIR_IN
                r->type = REDIR_IN;
            }
            r = r->next;
        }
        cmd = cmd->next;
    }
}

int write_heredoc_to(const char *path, const char *limiter)
{
    int   fd;
    char *line;

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (-1);

    // Le parent doit accepter Ctrl-C pendant le heredoc
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
            break;

        if (strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }

        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }

    close(fd);

    // On remet les signaux normaux du shell
    init_signals();

    return (0);
}
char    *generate_heredoc_tmp_name(void)
{
    static int  index = 0;
    char        *path;

    path = malloc(50);
    if (!path)
        return (NULL);

    sprintf(path, "/tmp/minishell_heredoc_%d", index++);
    return (path);
}
