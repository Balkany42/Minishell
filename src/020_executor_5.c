/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   020_executor_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 02:18:41 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/17 00:03:09 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	apply_redirs(t_cmd *cmd)
{
	t_redir	*last_in;

	last_in = find_last_in(cmd->redirs);
	if (apply_out_redirs(cmd->redirs) < 0)
		return (-1);
	if (apply_last_in(last_in) < 0)
		return (-1);
	return (0);
}

t_redir	*find_last_in(t_redir *r)
{
	t_redir	*last_in;

	last_in = NULL;
	while (r)
	{
		if (r->type == REDIR_IN)
			last_in = r;
		r = r->next;
	}
	return (last_in);
}

int	apply_out_redirs(t_redir *r)
{
	while (r)
	{
		if (r->type != REDIR_IN)
		{
			if (handle_normal_redir(r) < 0)
				return (-1);
		}
		r = r->next;
	}
	return (0);
}

int	apply_last_in(t_redir *last_in)
{
	int	fd;

	if (!last_in)
		return (0);
	fd = open(last_in->file, O_RDONLY);
	if (fd < 0)
	{
		perror(last_in->file);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_normal_redir(t_redir *r)
{
	int	fd;

	if (r->type == REDIR_IN)
		fd = open(r->file, O_RDONLY);
	else if (r->type == REDIR_OUT)
		fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(r->file);
		return (-1);
	}
	if (r->type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
