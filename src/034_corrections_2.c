/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   033_corrections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 04:28:19 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/17 00:08:16 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	write_heredoc_to(const char *path, const char *limiter)
{
	int		fd;
	char	*line;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			return (printf("bash: warning: here-document delimited "
					"by end-of-file (wanted `%s`)\n", limiter),
				rl_free_line_state(), close(fd), 1);
		if (strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (0);
}

char	*generate_heredoc_tmp_name(void)
{
	static int	index;
	char		*path;

	index = 0;
	path = malloc(50);
	if (!path)
		return (NULL);
	sprintf(path, "/tmp/minishell_heredoc_%d", index++);
	return (path);
}
