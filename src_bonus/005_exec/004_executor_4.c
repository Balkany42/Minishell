/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   016_executor_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrager <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 02:12:38 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/13 02:15:50 by mgrager          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell.h"

void	exec_external(t_cmd *cmd, t_minishell *sh)
{
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (ft_strchr(cmd->args[0], '/'))
		exec_with_path(cmd, sh);
	else
		exec_from_envpath(cmd, sh);
}

void	exec_with_path(t_cmd *cmd, t_minishell *sh)
{
	check_is_directory(cmd->args[0]);
	if (access(cmd->args[0], X_OK) != 0)
	{
		perror(cmd->args[0]);
		exit(126);
	}
	execve(cmd->args[0], cmd->args, sh->envp);
	perror(cmd->args[0]);
	exit(126);
}

void	exec_from_envpath(t_cmd *cmd, t_minishell *sh)
{
	char	*cmdpath;

	cmdpath = path(cmd->args[0], sh->envp);
	if (!cmdpath)
	{
		write(2, cmd->args[0], ft_strlen_2(cmd->args[0]));
		write(2, ": command not found\n", 21);
		exit(127);
	}
	if (access(cmdpath, X_OK) != 0)
	{
		perror(cmd->args[0]);
		free(cmdpath);
		exit(126);
	}
	execve(cmdpath, cmd->args, sh->envp);
	perror(cmd->args[0]);
	free(cmdpath);
	exit(127);
}

void	check_is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		write(2, path, ft_strlen(path));
		write(2, ": Is a directory\n", 17);
		exit(126);
	}
}
