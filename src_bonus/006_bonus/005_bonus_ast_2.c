/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   005_bonus_ast_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 08:31:46 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 13:44:47 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

t_ast	*parse_and_or(t_token **t, t_minishell *sh)
{
	t_ast	*left;
	t_ast	*node;

	left = parse_command_ast(t, sh);
	if (!left)
		return (NULL);
	while (*t && (*t)->type != RPAREN
		&& ((*t)->type == AND_IF || (*t)->type == OR_IF))
	{
		node = parse_and_or_step(left, t, sh);
		if (!node)
			return (NULL);
		left = node;
	}
	return (left);
}

t_ast	*parse_and_or_step(t_ast *left, t_token **t, t_minishell *sh)
{
	t_ast_op	op;
	t_ast		*right;
	t_ast		*node;

	if ((*t)->type == AND_IF)
		op = AST_AND_IF;
	else
		op = AST_OR_IF;
	*t = (*t)->next;
	right = parse_command_ast(t, sh);
	if (!right)
		return (NULL);
	node = malloc(sizeof(t_ast));
	node->op = op;
	node->left = left;
	node->right = right;
	node->cmd = NULL;
	return (node);
}

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

void	append_cmd_list(t_cmd *list, t_cmd *to_append)
{
	t_cmd	*tmp;

	if (!list)
		return ;
	tmp = list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = to_append;
}
