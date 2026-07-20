/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   004_bonus_ast_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 08:32:16 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 08:32:17 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

t_ast	*parse_command_ast(t_token **t, t_minishell *sh)
{
	t_cmd	*cmd_list;
	t_ast	*node;

	if (*t && (*t)->type == LPAREN)
		return (parse_group_ast(t, sh));
	cmd_list = parse_pipeline_segment(t, sh);
	node = malloc(sizeof(t_ast));
	node->op = AST_NONE;
	node->left = NULL;
	node->right = NULL;
	node->cmd = cmd_list;
	return (node);
}

t_cmd	*cmd_from_group(t_ast *group)
{
	t_cmd	*cmd;
	char	*code;

	cmd = cmd_new();
	add_arg(cmd, ft_strdup("sh"));
	add_arg(cmd, ft_strdup("-c"));
	code = ast_to_string(group);
	add_arg(cmd, code);
	return (cmd);
}

t_ast	*parse_group_ast(t_token **t, t_minishell *sh)
{
	t_ast	*inner;
	t_ast	*group;

	if (!*t || (*t)->type != LPAREN)
		return (NULL);
	*t = (*t)->next;
	inner = parse_and_or(t, sh);
	if (!inner)
		return (NULL);
	if (!*t || (*t)->type != RPAREN)
		return (NULL);
	*t = (*t)->next;
	group = malloc(sizeof(t_ast));
	group->op = AST_GROUP;
	group->left = inner;
	group->right = NULL;
	group->cmd = cmd_new();
	while (*t && is_redir((*t)->type))
		*t = parse_redirection_token(group->cmd, *t, sh);
	return (group);
}

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
