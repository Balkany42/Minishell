/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 04:30:56 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 04:24:07 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	run_heredoc_child(char *tmp, char *limiter)
{
	int	ret;

	rl_catch_signals = 1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	ret = write_heredoc_to(tmp, limiter);
	if (ret == 1)
		exit(1);
	if (ret < 0)
		exit(2);
	exit(0);
}

static int	handle_heredoc_status(int status, char *tmp, t_minishell *sh)
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
		sh->heredoc_interrupted = 1;
		free(tmp);
		return (0);
	}
	return (1);
}

static int	process_single_heredoc(t_redir *r, t_minishell *sh)
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
