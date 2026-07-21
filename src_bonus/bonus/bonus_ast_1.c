/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_ast_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 08:32:16 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:40:25 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static t_ast	*parse_group_or_pipeline(t_token **t, t_minishell *sh)
{
	t_ast	*g;
	t_cmd	*cmd_list;
	t_cmd	*rest;
	t_ast	*node;

	g = parse_group_ast(t, sh);
	if (!g)
		return (NULL);
	if (*t && (*t)->type == PIPE)
	{
		cmd_list = NULL;
		add_cmd_to_list(&cmd_list, cmd_from_group(g));
		*t = (*t)->next;
		rest = parse_pipeline_segment(t, sh);
		append_cmd_list(cmd_list, rest);
		node = malloc(sizeof(t_ast));
		node->op = AST_NONE;
		node->left = NULL;
		node->right = NULL;
		node->cmd = cmd_list;
		return (node);
	}
	return (g);
}

static t_ast	*parse_simple_command_ast(t_token **t, t_minishell *sh)
{
	t_cmd	*cmd_list;
	t_ast	*node;

	cmd_list = parse_pipeline_segment(t, sh);
	node = malloc(sizeof(t_ast));
	node->op = AST_NONE;
	node->left = NULL;
	node->right = NULL;
	node->cmd = cmd_list;
	return (node);
}

t_ast	*parse_command_ast(t_token **t, t_minishell *sh)
{
	if (*t && (*t)->type == LPAREN)
		return (parse_group_or_pipeline(t, sh));
	return (parse_simple_command_ast(t, sh));
}

t_cmd	*cmd_from_group(t_ast *group)
{
	t_cmd	*cmd;

	cmd = cmd_new();
	cmd->group_ast = group;
	if (group->cmd)
		cmd->redirs = group->cmd->redirs;
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
