/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   017_executor_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 02:18:41 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 02:23:11 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	apply_redirs(t_cmd *cmd)
{
	t_redir	*r;
	int		fd;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_IN)
		{
    		fd = open(r->file, O_RDONLY);
    		if (fd < 0)
        	return (-1);
    		dup2(fd, STDIN_FILENO);
    		close(fd);
		}
		else
		{
			if (handle_normal_redir(r) < 0)
				return (-1);
		}
		r = r->next;
	}
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
		return (-1);
	if (r->type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

// int	open_heredoc(char *limiter)
// {
// 	int	pipefd[2];

// 	if (pipe(pipefd) == -1)
// 		return (-1);
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_IGN);
// 	if (read_heredoc_lines(pipefd[1], limiter) < 0)
// 	{
// 		close(pipefd[1]);
// 		close(pipefd[0]);
// 		return (-1);
// 	}
// 	close(pipefd[1]);
// 	init_signals();
// 	return (pipefd[0]);
// }

// int	read_heredoc_lines(int write_fd, char *limiter)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("heredoc> ");
// 		if (!line)
// 			return (-1);
// 		if (ft_strcmp(line, limiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(write_fd, line, ft_strlen(line));
// 		write(write_fd, "\n", 1);
// 		free(line);
// 	}
// 	return (0);
// }
