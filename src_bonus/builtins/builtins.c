/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrelli <melchior.grellier42@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 03:15:36 by mgrager           #+#    #+#             */
/*   Updated: 2026/07/21 01:54:17 by megrelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell_bonus.h"

static int	handle_special_builtins(char **args, t_minishell *sh)
{
	if (ft_strcmp(args[0], ":") == 0)
	{
		sh->exit_status = 0;
		return (1);
	}
	if (ft_strcmp(args[0], "!") == 0)
	{
		sh->exit_status = 1;
		return (1);
	}
	if (ft_strcmp(args[0], ".") == 0)
	{
		printf(".: filename argument required\n");
		sh->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	handle_normal_builtins(t_cmd *cmd, t_minishell *sh)
{
	char	**args;

	args = cmd->args;
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args, sh));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, sh));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args, sh));
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(cmd, sh));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(cmd, sh));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(args, sh));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, sh));
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, ":") == 0 || ft_strcmp(cmd, "!") == 0
		|| ft_strcmp(cmd, ".") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_minishell *sh)
{
	char	**args;

	args = cmd->args;
	if (handle_special_builtins(args, sh))
		return (1);
	return (handle_normal_builtins(cmd, sh));
}
