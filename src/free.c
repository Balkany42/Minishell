#include "../include/minishell.h"

void free_tab(char **tab)
{
    int i = 0;

    if (!tab)
        return;

    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}
void free_cmds(t_cmd *cmds)
{
    t_cmd *tmp;
    int     i;

    while (cmds)
    {
        tmp = cmds->next;

        // Libérer args
        if (cmds->args)
        {
            i = 0;
            while (cmds->args[i])
            {
                free(cmds->args[i]);
                i++;
            }
            free(cmds->args);
        }

        // Libérer redirections si tu en as
        t_redir *r = cmds->redirs;
        t_redir *next_r;
        while (r)
        {
            next_r = r->next;
            if (r->file)
                free(r->file);
            free(r);
            r = next_r;
        }

        // Libérer la structure elle-même
        free(cmds);
        cmds = tmp;
    }
}
