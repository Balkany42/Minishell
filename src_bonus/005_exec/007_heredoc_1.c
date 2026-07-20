/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   033_corrections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 04:30:56 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/17 00:57:09 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

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

void	prepare_heredocs(t_cmd *cmds, t_minishell *sh)
{
	t_cmd	*cmd;
	t_redir	*r;

	cmd = cmds;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				if (!process_single_heredoc(r, sh))
					return ;
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
}

void	run_heredoc_child(char *tmp, char *limiter)
{
	rl_catch_signals = 1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	if (write_heredoc_to(tmp, limiter) < 0)
		exit(1);
	exit(0);
}

int	handle_heredoc_status(int status, char *tmp, t_minishell *sh)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		sh->exit_status = 130;
		sh->heredoc_interrupted = 1;
		free(tmp);
		printf("heredoc>\n");
		return (0);
	}
	if (WEXITSTATUS(status) != 0)
	{
		sh->exit_status = 1;
		free(tmp);
		return (0);
	}
	return (1);
}

int	process_single_heredoc(t_redir *r, t_minishell *sh)
{
	char	*tmp;
	int		status;
	pid_t	pid;

	tmp = generate_heredoc_tmp_name();
	if (!tmp)
	{
		sh->exit_status = 1;
		return (0);
	}
	pid = fork();
	if (pid == 0)
		run_heredoc_child(tmp, r->file);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handler_sigint);
	if (!handle_heredoc_status(status, tmp, sh))
		return (0);
	free(r->file);
	r->file = tmp;
	r->type = REDIR_IN;
	return (1);
}
