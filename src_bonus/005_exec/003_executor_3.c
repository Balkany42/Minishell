/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   018_executor_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:31:28 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/20 03:44:33 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

void	pipeline_child(t_cmd *cmd, t_minishell *sh, int prev_fd, int pipefd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->args || !cmd->args[0])
		pipeline_child_empty(cmd);
	pipeline_child_pipes(cmd, prev_fd, pipefd);
	if (apply_redirs(cmd) == -1)
		exit(1);
	if (is_builtin(cmd->args[0]))
	{
		exec_builtin(cmd, sh);
		exit(sh->exit_status);
	}
	exec_external(cmd, sh);
	exit(127);
}

void	pipeline_child_empty(t_cmd *cmd)
{
	char	buffer[4096];
	ssize_t	n;

	if (cmd->redirs)
	{
		if (apply_redirs(cmd) == -1)
			exit(1);
		n = read(STDIN_FILENO, buffer, sizeof(buffer));
		while (n > 0)
		{
			write(STDOUT_FILENO, buffer, n);
			n = read(STDIN_FILENO, buffer, sizeof(buffer));
		}
	}
	exit(0);
}

void	pipeline_child_pipes(t_cmd *cmd, int prev_fd, int pipefd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

void	pipeline_parent(t_cmd *cmd, int *prev_fd, int pipefd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

void	pipeline_wait_last(pid_t last_pid, t_minishell *sh)
{
	int		status;
	pid_t	p;

	p = waitpid(-1, &status, 0);
	while (p > 0)
	{
		if (p == last_pid)
		{
			if (WIFEXITED(status))
				sh->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				sh->exit_status = 128 + WTERMSIG(status);
		}
		p = waitpid(-1, &status, 0);
	}
}
