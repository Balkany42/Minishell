/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 03:37:00 by megrelli          #+#    #+#             */
/*   Updated: 2025/12/29 16:39:05 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_read_and_stash(int fd, char *stash)
{
	int		bytes;
	char	*buffer;
	char	*temp;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free(stash), NULL);
	bytes = 1;
	while (bytes > 0 && !ft_strchr(stash, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (ft_free(stash), ft_free(buffer), NULL);
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		temp = ft_strjoin(stash, buffer);
		ft_free(stash);
		stash = temp;
		if (!stash)
			return (ft_free(buffer), NULL);
	}
	ft_free(buffer);
	return (stash);
}

static char	*ft_extract_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	if (!ft_strchr(stash, '\n'))
	{
		line = ft_strjoin(stash, "");
		if (!line)
			return (NULL);
		return (line);
	}
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*ft_clean_stash(char *stash)
{
	int		i;
	char	*new_stash;

	if (!ft_strchr(stash, '\n'))
		return (ft_free(stash), stash = NULL);
	i = 0;
	while (stash[i] != '\n')
		i++;
	i++;
	new_stash = ft_strjoin(&stash[i], "");
	ft_free(stash);
	if (!new_stash || new_stash[0] == '\0')
		return (ft_free(new_stash), NULL);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
	{
		stash = ft_strjoin("", "");
		if (!stash)
			return (NULL);
	}
	stash = ft_read_and_stash(fd, stash);
	if (!stash)
		return (NULL);
	if (stash[0] == '\0')
	{
		return (ft_free(stash), stash = NULL);
	}
	line = ft_extract_line(stash);
	if (!line)
		return (ft_free(stash), stash = NULL);
	stash = ft_clean_stash(stash);
	return (line);
}
