/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 23:56:11 by megrelli          #+#    #+#             */
/*   Updated: 2026/06/18 19:47:13 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		count_args(t_token *list)
{
	int	count;
	
	count = 0;
	while (list && list->type != PIPE)
	{
		if (list->type == WORD)
			count++;
		else
		{
			if (list->next != NULL && list->next->type != PIPE)
				list = list->next;
		}
		list = list->next;
	}
	return (count);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd_to_list(t_cmd **cmd_list, t_cmd *cmd)
{
	t_cmd	*temp;
	
	if (!*cmd_list)
	{
		*cmd_list = cmd;
		return ;
	}
	temp = *cmd_list;
	while (temp->next)
		temp = temp->next;
	temp->next = cmd;
}
void add_arg(t_cmd *cmd, char *arg)
{
    int     count = 0;
    char    **new_args;

    while (cmd->args && cmd->args[count])
        count++;

    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return;

    for (int i = 0; i < count; i++)
        new_args[i] = cmd->args[i];

    new_args[count] = arg;
    new_args[count + 1] = NULL;

    free(cmd->args);
    cmd->args = new_args;
}

void add_redir(t_cmd *cmd, t_tokentype type, char *file)
{
    t_redir *new = malloc(sizeof(t_redir));
    t_redir *tmp;

    if (!new)
        return;

    new->type = type;
    new->file = file;
    new->next = NULL;

    if (!cmd->redirs)
    {
        cmd->redirs = new;
        return;
    }
    tmp = cmd->redirs;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}
t_cmd *parse_command(t_token **list, t_minishell *sh)
{
    t_cmd *cmd = cmd_new();
    t_token *tok = *list;

    cmd->redirs = NULL;

    while (tok && tok->type != PIPE)
    {
        /* ---------------- ARGUMENTS ---------------- */
        if (tok->type == WORD)
        {
            char *expanded = expand_token(tok, sh);   // 🔥 expansion correcte
            add_arg(cmd, expanded);
            tok = tok->next;
        }

        /* ---------------- REDIRECTIONS ---------------- */
        else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
              || tok->type == APPEND || tok->type == HEREDOC)
        {
            t_tokentype type = tok->type;
            tok = tok->next;

            if (!tok || tok->type != WORD)
            {
                free_cmds(cmd);
                return NULL;
            }

            char *expanded = expand_token(tok, sh);   // 🔥 expansion correcte

            // ambiguous redirect (bash rule)
            if (type != HEREDOC && (!expanded[0] || ft_strchr(expanded, ' ')))
            {
                printf("minishell: %s: ambiguous redirect\n", tok->value);
                sh->exit_status = 1;
                free_cmds(cmd);
                return NULL;
            }

            add_redir(cmd, type, expanded);  // 🔥 on stocke l’expansion
            tok = tok->next;
        }

        /* ---------------- ERREUR ---------------- */
        else
        {
            free_cmds(cmd);
            return NULL;
        }
    }

    *list = tok;
    return cmd;
}


t_cmd *parser(t_token *list, t_minishell *sh)
{
    t_cmd *head = NULL;
    t_cmd *cmd;

    while (list)
    {
        cmd = parse_command(&list, sh);   // 🔥 correction ici
        if (!cmd)
            return NULL;

        add_cmd_to_list(&head, cmd);

        if (list && list->type == PIPE)
            list = list->next;
    }
    return head;
}
