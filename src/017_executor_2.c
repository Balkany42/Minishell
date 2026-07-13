/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   014_executor_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:26:48 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 01:29:35 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_single(t_cmd *cmd, t_minishell *sh)
{
	pid_t	pid;

	if (is_builtin(cmd->args[0]))
		return (exec_single_builtin(cmd, sh));
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		exec_single_child(cmd, sh);
	exec_single_parent(pid, sh);
}

void	exec_single_builtin(t_cmd *cmd, t_minishell *sh)
{
	if (apply_redirs(cmd) == -1)
		return ;
	exec_builtin(cmd, sh);
}

void	exec_single_child(t_cmd *cmd, t_minishell *sh)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirs(cmd) == -1)
		exit(1);
	exec_external(cmd, sh);
	exit(127);
}

void	exec_single_parent(pid_t pid, t_minishell *sh)
{
	int status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->exit_status = 128 + WTERMSIG(status);
}
