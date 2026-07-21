/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 04:28:19 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 04:18:23 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

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
				rl_on_new_line(), close(fd), 1);
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
	static int	index = 0;
	char		*num;
	char		*path;

	num = ft_itoa(index++);
	if (!num)
		return (NULL);
	path = ft_strjoin("/tmp/minishell_heredoc_", num);
	free(num);
	return (path);
}
