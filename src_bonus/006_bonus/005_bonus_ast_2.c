/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   005_bonus_ast_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 08:31:46 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 08:31:47 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

t_cmd	*parse_pipeline_segment(t_token **list, t_minishell *sh)
{
	t_cmd	*head;
	t_cmd	*ret;

	head = NULL;
	while (*list)
	{
		if ((*list)->type == AND_IF || (*list)->type == OR_IF
			|| (*list)->type == RPAREN)
			break ;
		sh->redir_error = 0;
		ret = parse_pipeline_step(list, sh, &head);
		if (!ret)
			return (NULL);
		if (ret == (t_cmd *)1)
			break ;
	}
	return (head);
}

t_cmd	*parse_pipeline_step(t_token **list, t_minishell *sh, t_cmd **head)
{
	t_cmd	*cmd;
	t_ast	*g;

	if ((*list)->type == LPAREN)
	{
		g = parse_group_ast(list, sh);
		cmd = cmd_from_group(g);
	}
	else
		cmd = parse_command(list, sh);
	if (!cmd)
		return (NULL);
	add_cmd_to_list(head, cmd);
	if (*list && (*list)->type == PIPE)
		*list = (*list)->next;
	else
		return ((t_cmd *)1);
	return (cmd);
}

void	prepare_heredocs_ast(t_ast *node, t_minishell *sh)
{
	if (!node)
		return ;
	if (node->op == AST_NONE)
	{
		prepare_heredocs(node->cmd, sh);
		return ;
	}
	if (node->op == AST_GROUP)
	{
		if (node->cmd)
			prepare_heredocs(node->cmd, sh);
		prepare_heredocs_ast(node->left, sh);
		return ;
	}
	prepare_heredocs_ast(node->left, sh);
	prepare_heredocs_ast(node->right, sh);
}
