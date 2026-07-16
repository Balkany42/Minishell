/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   010_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 23:56:11 by megrelli          #+#    #+#             */
/*   Updated: 2026/07/13 00:33:06 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*parser(t_token *list, t_minishell *sh)
{
	t_cmd	*head;
	t_cmd	*cmd;

	head = NULL;
	while (list)
	{
		sh->redir_error = 0;
		cmd = parse_command(&list, sh);
		if (!cmd)
			return (NULL);
		add_cmd_to_list(&head, cmd);
		if (list && list->type == PIPE)
				list = list->next;
	}
	return (head);
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
