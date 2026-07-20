/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   017_executor_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 01:26:48 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/17 00:53:07 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_pipeline(t_cmd *cmds, t_minishell *sh)
{
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = -1;
	exec_pipeline_loop(cmds, sh, &prev_fd, &last_pid);
	pipeline_wait_last(last_pid, sh);
	init_signals();
}

void	exec_pipeline_loop(t_cmd *cmds, t_minishell *sh,
			int *prev_fd, pid_t *last_pid)
{
	t_pipectx	ctx;

	ctx.sh = sh;
	ctx.prev_fd = prev_fd;
	ctx.last_pid = last_pid;
	while (cmds)
	{
		if (!exec_pipeline_step(cmds, &ctx))
			return ;
		cmds = cmds->next;
	}
}

int	exec_pipeline_step(t_cmd *cmds, t_pipectx *ctx)
{
	pid_t	pid;

	if (cmds->next && pipe(ctx->pipefd) == -1)
	{
		if (*ctx->prev_fd != -1)
			close(*ctx->prev_fd);
		perror("minishell: pipe");
		ctx->sh->exit_status = 1;
		return (0);
	}
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
		pipeline_child(cmds, ctx->sh, *ctx->prev_fd, ctx->pipefd);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (!cmds->next)
			*ctx->last_pid = pid;
		pipeline_parent(cmds, ctx->prev_fd, ctx->pipefd);
	}
	return (1);
}
