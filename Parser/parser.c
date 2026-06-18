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

t_cmd	*parse_tokens(t_token *list, char **envp)
{
	t_cmd	*head;
	t_cmd	*curr;
	int		count;
	int		i;

	head = NULL;
	curr = NULL;
	while (list)
	{
		curr = cmd_new();
		if (!curr)
			return (NULL);
		count = count_args(list);
		curr->args = malloc(sizeof(char *) * (count + 1));
		if (!curr->args)
			return (NULL);
		i = 0;
		while (list && list->type != PIPE)
		{
			if (list->type == WORD)
			{
				curr->args[i] = expander(list->value);
				if (!curr->args[i++])
					return (NULL);
			}
			else if (list->next != NULL && list->next->type != PIPE)
				list = list->next;
			list = list->next;
		}
		curr->args[i] = NULL;
		add_cmd_to_list(&head, curr);
		if (list && list->type == PIPE)
			list = list->next;
	}
	return (head);
}
/*
void print_cmds(t_cmd *cmds)
{
    t_cmd   *temp;
    int     i;
    int     cmd_num;

    temp = cmds;
    cmd_num = 1;
    printf("\n🚀 --- DEBUT DU PARSING D'EXECUTION --- 🚀\n");
    while (temp != NULL)
    {
        printf("📦 BOITE COMMANDE [%d]\n", cmd_num);
        printf("   ➡️ fd_in  : %d\n", temp->fd_in);
        printf("   ⬅️ fd_out : %d\n", temp->fd_out);
        
        i = 0;
        if (temp->args != NULL)
        {
            while (temp->args[i] != NULL)
            {
                printf("   📝 arg[%d] : [%s]\n", i, temp->args[i]);
                i++;
            }
        }
        else
        {
            printf("   📝 args   : NULL\n");
        }
        
        temp = temp->next;
        cmd_num++;
        printf("----------------------------------------\n");
    }
    printf("✅ --- FIN DU PARSING ---\n\n");
}*/
