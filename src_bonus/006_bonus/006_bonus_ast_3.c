/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   006_bonus_ast_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 08:31:53 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 08:31:55 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

int	exec_ast(t_ast *node, t_minishell *sh)
{
	if (!node)
		return (1);
	if (node->op == AST_NONE)
		return (exec_none_ast(node, sh));
	if (node->op == AST_GROUP)
		return (exec_group_ast(node, sh));
	if (node->op == AST_AND_IF || node->op == AST_OR_IF)
		return (exec_and_or_ast(node, sh));
	return (1);
}

int	exec_none_ast(t_ast *node, t_minishell *sh)
{
	t_cmd	*tmp;

	if (node->cmd == NULL)
		return (exec_ast(node->left, sh));
	tmp = node->cmd;
	while (tmp)
	{
		executor(tmp, sh);
		tmp = tmp->next;
	}
	return (sh->exit_status);
}

int	exec_group_ast(t_ast *node, t_minishell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (node->cmd && node->cmd->redirs)
			if (apply_redirs(node->cmd) < 0)
				exit(1);
		status = exec_ast(node->left, sh);
		exit(status);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	exec_and_or_ast(t_ast *node, t_minishell *sh)
{
	int	left;

	left = exec_ast(node->left, sh);
	if (node->op == AST_AND_IF)
	{
		if (left == 0)
			return (exec_ast(node->right, sh));
		return (left);
	}
	if (node->op == AST_OR_IF)
	{
		if (left != 0)
			return (exec_ast(node->right, sh));
		return (left);
	}
	return (1);
}
