/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   019_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 02:43:47 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 02:47:41 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/minishell.h"

char	*path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full;
	int		i;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (check_direct_path(cmd));
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full = search_in_paths(paths, cmd);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (full);
}

char	*check_direct_path(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (full && access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}
